#include "ate.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <utility>

#include <recognition/factory.h>
#include <opencv2/opencv.hpp>

#include "adapters/db_manager_factory.h"
#include "ate_error.h"
#include "common.h"
#include "exceptions.h"
#include "factory/recognition_factory.h"
#include "interaction/SPI/spi_interaction.h"
#include "interaction/dummy/dummy_interaction.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "utils/env_config.h"
#include "utils/video_status.h"
#include "video_streaming/streamer_factory.h"

using namespace defines;
namespace fs = std::experimental::filesystem;

namespace {

detector::DetectorTypes StrToDetectorType(const std::string& detector_type) {
  if (kTemplateMathcing == detector_type) {
    return detector::DetectorTypes::kTemplateDetector;
  }
  if (kMultiscaleTemplateMatching == detector_type) {
    return detector::DetectorTypes::kMultiscaleTemplateDetector;
  }
  logger::error("Undefined type of Detector \'{}\'.", detector_type);
  logger::info("Use default detector type \'{}\'", kTemplateMathcing);
  return detector::DetectorTypes::kTemplateDetector;
}

std::unique_ptr<interaction::Interaction> InteractionFactory(const std::string& interaction_type) {
  if (interaction_type == kSpi) {
    cv::Size frame_size{common::Config().GetInt(kVideoStreamSection, kFrameWidthOption, {}),
                        common::Config().GetInt(kVideoStreamSection, kFrameHeightOption, {})};

    return std::make_unique<interaction::SpiInteraction>(utils::GetEnvSettings(utils::EnvSettings::kSpiDeviceAddress),
                                                         frame_size.width, frame_size.height);
  }

  if (interaction_type == kDummy) {
    logger::critical("[ATE] Used DummyInteraction for debugging only");
    return std::make_unique<interaction::DummyInteraction>();
  }

  logger::critical("[ATE] Undefined type of interaction");
  throw interaction::InteractionTypeError{};
}

std::unique_ptr<utils::ScreenshotRecorder> MakeScreenshotRecorder() {
  std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder;
  try {
    screenshot_recorder = std::make_unique<utils::ScreenshotRecorder>(
        common::Config().GetBool(kScreenshotRecorderSection, kScreenshotOption, false),
        common::Config().GetString(kScreenshotRecorderSection, kScreenshotDirectoryOption, {}));
  } catch (const std::runtime_error& e) {
    logger::info("[ATE] Screenshot recorder was not created, cause: {}.", e.what());
  }
  return screenshot_recorder;
}

std::error_code ScreenshotErrorToStdErrorCode(utils::ScreenshotError screenshot_error) {
  switch (screenshot_error) {
    case utils::ScreenshotError::kEmptyFileName:
      return common::make_error_code(common::AteError::kEmptyFileName);

    case utils::ScreenshotError::kSystemError:
      return common::make_error_code(common::AteError::kSystemError);

    case utils::ScreenshotError::kWrongExtension:
      return common::make_error_code(common::AteError::kWrongExtension);

    case utils::ScreenshotError::kPermissionDenied:
      return common::make_error_code(common::AteError::kPermissionDenied);

    case utils::ScreenshotError::kImageAssemblingFailed:
      return common::make_error_code(common::AteError::kImageAssemblingFailed);

    case utils::ScreenshotError::kNoAvailableDiskSpace:
      return common::make_error_code(common::AteError::kNoAvailableDiskSpace);

    case utils::ScreenshotError::kSuccess:
      return {};
  }

  assert(!"Unhandled ScreenshotError code");
  return common::make_error_code(common::AteError::kSystemError);
}

}  // namespace

ATE::ATE()
    : interaction_{InteractionFactory(common::Config().GetString(kInteraction, kInteractionType, {}))},
      matcher_{
          streamer::MakeStreamer(),
          detector::MakeImageDetector(
              StrToDetectorType(common::Config().GetString(kImageDetectorSection, kImageDetectorMatchingType,
                                                           kMultiscaleTemplateMatching)),
              common::Config().GetDouble(kImageDetectorSection, kImageDetectorConfidenceOption, {})),
          factory::CreateTextDetector(common::Config().GetString(kDBSection, kTargetOption, {})),
          MakeScreenshotRecorder(),
          std::make_unique<utils::GpioVideoStatus>(utils::GetEnvSettings(utils::EnvSettings::kGpioVideoStatusPath))} {
  // Init storage
  auto storage_result =
      storage_.Init(adapter::CreateDBManager(), common::Config().GetString(kDBSection, kTargetOption, {}),
                    common::Config().GetString(kDBSection, kBuildOption, {}),
                    common::Config().GetString(kDBSection, kCollectionModeOption, {}));
  if (storage_result != adapter::DBManagerError::kSuccess) {
    logger::critical("[ATE] failed to initialize storage. Error: {}", static_cast<int>(storage_result));
    throw storage::ConnectionFailure{};
  }
}

ATE::~ATE() = default;

void ATE::TapObject(const cv::Point& point) { interaction_->Tap(point.x, point.y); }

std::error_code ATE::LongPress(uint16_t x, uint16_t y, const std::chrono::milliseconds& timeout) {
  const std::chrono::seconds kMaxLongPressDelay(60);
  std::error_code long_press_error;

  if (timeout > kMaxLongPressDelay) {
    return common::make_error_code(common::AteError::kInvalidDuration);
  }
  interaction_->Press(x, y);
  std::this_thread::sleep_for(timeout);
  interaction_->Release(x, y);
  return std::error_code{};
}

void ATE::TouchAndDrag(const std::string& object_or_name, const cv::Point& start_point, const cv::Point& delta_point) {
  logger::trace("objectOrName '{}' has been draged from x: {} y: {} by dx: {} dy: {}", object_or_name, start_point.x,
                start_point.y, delta_point.x, delta_point.y);

  interaction_->TouchAndDrag(start_point.x, start_point.y, delta_point.x, delta_point.y);
}

void ATE::PressAndHold(const cv::Point& point) { interaction_->Press(point.x, point.y); }

void ATE::PressRelease(const cv::Point& point) { interaction_->Release(point.x, point.y); }

std::pair<cv::Rect, std::error_code> ATE::WaitForObject(const std::string& object_or_name,
                                                        const std::chrono::milliseconds& timeout,
                                                        const cv::Rect& search_region) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;
  auto pattern = storage_.GetItem(object_or_name);
  const bool is_image = !pattern.empty();
  cv::Rect match_area;
  std::error_code match_error;

  do {
    std::tie(match_area, match_error) = is_image ? matcher_.DetectImage(object_or_name, pattern, search_region)
                                                 : matcher_.DetectText(object_or_name, search_region);
  } while (match_error == common::AteError::kPatternNotFound && std::chrono::steady_clock::now() <= timeout_point);

  return {match_area, match_error};
}

std::pair<cv::Rect, std::error_code> ATE::WaitForObject(const common::ObjectDataIdentity& object_data_identity,
                                                        const std::chrono::milliseconds& timeout,
                                                        const cv::Rect& search_region) {
  cv::Rect match_area{};
  std::error_code match_error{common::AteError::kPatternNotFound};

  const auto objects = storage_.GetItemDataByWildcard(object_data_identity);
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;

  // Match all objects at least once before checking the timeout
  do {
    std::find_if(objects.cbegin(), objects.cend(), [this, &match_area, &match_error, &search_region](const auto& object) {
      const auto pattern = storage_.GetItem(object.name, object.sync_version, object.build_version, object.mode);

      if (!pattern.empty()) {
        std::tie(match_area, match_error) = matcher_.DetectImage(object.name, pattern, search_region);
      }

      // We search for either first success or first fatal error. kPatternNotFound is interpreted as 'retry'
      return match_error != common::AteError::kPatternNotFound;
    });
  } while (match_error == common::AteError::kPatternNotFound && std::chrono::steady_clock::now() <= timeout_point);

  return {match_area, match_error};
}

void ATE::ChangeResolution(int x, int y) {
  matcher_.ChangeResolution(x, y);
  interaction_->ChangeResolution(x, y);
}

adapter::DBManagerError ATE::ChangeSyncVersion(const std::string& sync_version, const std::string& sync_build_version) {
  auto result = storage_.ChangeSyncVersion(sync_version, sync_build_version);
  // If sync exists - change preprocessing list
  if (result == adapter::DBManagerError::kSuccess) {
    logger::info("Sync version changed to: {}. Set default preprocessing list for new sync version", sync_version);
    matcher_.ChangePreprocessingList(sync_version);
  }
  return result;
}

adapter::DBManagerError ATE::ChangeSyncMode(const std::string& collection_mode) {
  return storage_.ChangeCollectionMode(collection_mode);
}

std::error_code ATE::GetScreenshot(const std::string& path, const std::string& filename) {
  return matcher_.GetScreenshot(path, filename);
}

std::pair<std::string, std::error_code> ATE::GetText(const cv::Point& point, const cv::Point& delta_point) {
  return matcher_.GetText(point, delta_point);
}

std::vector<common::ObjectData> ATE::GetObjectsDataByPattern(const common::ObjectDataIdentity& pattern) {
  logger::debug("GetObjectsDataByPattern(wildcard)");
  return storage_.GetItemDataByWildcard(pattern);
}

std::vector<common::ObjectData> ATE::GetObjectsDataByPattern(const std::string& object_name) {
  logger::debug("GetObjectsDataByPattern(string)");
  return storage_.GetItemData(object_name);
}

std::pair<int, std::error_code> ATE::GetImagesDiscrepancy(const std::string& icon_path_second,
                                                          const std::string& icon_path_first,
                                                          const cv::Point& top_left_coordinate,
                                                          const cv::Point& bottom_right_coordinate) const {
  return matcher_.GetImagesDiscrepancy(icon_path_second, icon_path_first, top_left_coordinate, bottom_right_coordinate);
}

std::vector<std::string> ATE::CaptureFrames(size_t interval, size_t duration, const cv::Rect& area,
                                            const std::string& sub_path, std::error_code& error) {
  using namespace std::chrono_literals;
  constexpr const auto kLimitTime = 5s;

  if (std::chrono::milliseconds(duration) > kLimitTime) {
    error = common::make_error_code(common::AteError::kInvalidDuration);
    return {};
  }

  std::vector<std::string> result;
  result.reserve(0u == interval ? duration : duration / interval + 1);

  std::queue<std::pair<std::string, cv::Mat>> queue_frames;
  std::atomic_bool getting_frames{true};
  std::atomic_bool error_on_save{false};
  std::mutex mut;
  std::condition_variable cond_var;
  std::error_code error_code_on_save;

  // perform saving frames in the other thread
  std::thread save_thr(
      [sub_path, &queue_frames, &getting_frames, &error_on_save, &error_code_on_save, &result, &mut, &cond_var]() {
        std::vector<std::pair<std::string, cv::Mat>> frames;
        bool log_left_unsaved_frames{true};

        while (true) {
          if (getting_frames) {
            std::unique_lock<std::mutex> lk(mut);
            cond_var.wait(lk, [&getting_frames, &queue_frames]() { return !getting_frames || !queue_frames.empty(); });
            while (!queue_frames.empty()) {
              frames.emplace_back(std::move(queue_frames.front().first), std::move(queue_frames.front().second));
              queue_frames.pop();
            }
          } else {
            while (!queue_frames.empty()) {
              frames.emplace_back(std::move(queue_frames.front().first), std::move(queue_frames.front().second));
              queue_frames.pop();
            }
            // Log uses for extra debug info: how many frames kept in RAM after finishing of frame grabbing
            if (log_left_unsaved_frames) {
              log_left_unsaved_frames = false;
              logger::debug("Left unsaved frames in container after duration was end: {}\nFrame size: {} bytes",
                            frames.size(), frames.empty() ? 0 : frames[0].second.total() * frames[0].second.elemSize());
            }
          }

          for (auto& item : frames) {
            const auto screen_error = utils::ScreenshotRecorder::GetScreenshot(item.second, sub_path, item.first);
            if (utils::ScreenshotError::kSuccess != screen_error) {
              error_code_on_save = ScreenshotErrorToStdErrorCode(screen_error);
              error_on_save = true;
              break;
            }
            result.push_back(item.first);
          }

          frames.clear();

          if (error_on_save) {
            logger::error("Error occurred in the child thread");
            break;
          }

          if (!getting_frames && queue_frames.empty()) break;
        }
      });

  constexpr auto kCharSize = 100;
  char time_cstr[kCharSize];
  auto const time_point = std::chrono::steady_clock::now() + std::chrono::milliseconds(duration);
  auto elapsed_interval_processing = std::chrono::steady_clock::now();
  size_t counter = 0;
  std::string filename;
  bool at_least_one_grabbed{false};

  do {
    cv::Mat frame;
    error = matcher_.GetFrame(frame, area);
    if (!error) {
      // define file name as timestamp year_month_day_hours_minutes_seconds_(%d).png
      std::time_t t = std::time(nullptr);
      std::memset(time_cstr, 0, kCharSize * sizeof(*time_cstr));
      std::strftime(time_cstr, kCharSize, "%y_%m_%d_%H_%M_%S(", std::localtime(&t));
      filename = time_cstr;
      filename.append(std::to_string(++counter) + ").png");

      at_least_one_grabbed = true;

      std::lock_guard<std::mutex> lock(mut);
      queue_frames.emplace(filename, std::move(frame));
    } else {
      logger::error("Unable to get video frame");
      if (at_least_one_grabbed) {  // ignore this error if any frame was grabbed
        error.clear();
      }
      cond_var.notify_one();
      continue;
    }

    cond_var.notify_one();

    if (0u != interval) {
      const auto elapsed_ms = std::chrono::duration_cast<std::chrono::duration<size_t, std::milli>>(
                                  std::chrono::steady_clock::now() - elapsed_interval_processing)
                                  .count();
      if (elapsed_ms < interval) {
        elapsed_interval_processing = std::chrono::steady_clock::now();
        // track time spent on processing with specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(interval - elapsed_ms));
      } else {
        // the thread is late on processing, that means no sense to sleep if even interval > 0
        elapsed_interval_processing =
            std::chrono::steady_clock::now() - std::chrono::milliseconds(elapsed_ms - interval);
      }
    }
  } while (!error_on_save && std::chrono::steady_clock::now() < time_point);

  getting_frames = false;
  cond_var.notify_one();

  if (save_thr.joinable()) {
    save_thr.join();
  }

  if (error_on_save) {
    error = error ? error : error_code_on_save;
    logger::error("Caught error in main thread from child thread: {}", error_code_on_save.message());
  }

  // remove saved frames on error
  if (error && !result.empty()) {
    logger::warn("CaptureFrames is failed, next captured frames are going to be removed:");
    result.erase(
        std::remove_if(result.begin(), result.end(),
                       [&sub_path](const std::string& file) {
                         std::error_code error_rm_frame;
                         if (!fs::remove(fs::path(ATE_WRITABLE_DATA_PREFIX) / sub_path / file, error_rm_frame)) {
                           logger::error("Failed to remove '{}/{}/{}', error - {}", ATE_WRITABLE_DATA_PREFIX, sub_path,
                                         file, error_rm_frame.message());
                           return false;
                         }
                         logger::warn("File is successfully removed: '{}/{}/{}'", ATE_WRITABLE_DATA_PREFIX, sub_path,
                                      file);
                         return true;
                       }),
        result.end());
  }

  return result;
}

std::pair<std::vector<common::Rect>, std::error_code> ATE::FindAllImages(const std::string& object_or_name,
                                                                         const common::Rect& area) {
  auto pattern = storage_.GetItem(object_or_name);

  if (pattern.empty()) {
    return {{}, common::make_error_code(common::AteError::kPatternInvalid)};
  }

  std::vector<cv::Rect> found_objects_cv_areas;
  std::error_code match_error;

  std::tie(found_objects_cv_areas, match_error) =
      matcher_.DetectImages(object_or_name, pattern, cv::Rect(area.x, area.y, area.width, area.height));

  // transformation from cv::Rect to common::Rect
  std::vector<common::Rect> found_objects_common_areas;
  std::transform(
      found_objects_cv_areas.begin(), found_objects_cv_areas.end(), std::back_inserter(found_objects_common_areas),
      [](const cv::Rect& cv_rect) { return common::Rect(cv_rect.x, cv_rect.y, cv_rect.width, cv_rect.height); });

  return {found_objects_common_areas, match_error};
}

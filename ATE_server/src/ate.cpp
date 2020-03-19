#include "ate.h"

#include <stdexcept>
#include <utility>

#include <recognition/factory.h>
#include <opencv2/opencv.hpp>

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

}  // namespace

ATE::ATE()
    : interaction_{InteractionFactory(common::Config().GetString(kInteraction, kInteractionType, {}))},
      matcher_{streamer::MakeStreamer(),
               detector::MakeImageDetector(
                   StrToDetectorType(common::Config().GetString(kImageDetectorSection, kImageDetectorMatchingType,
                                                                kMultiscaleTemplateMatching)),
                   common::Config().GetDouble(kImageDetectorSection, kImageDetectorConfidenceOption, {})),
               factory::CreateTextDetector(common::Config().GetString(kDBSection, kTargetOption, {})),
               MakeScreenshotRecorder(), std::make_unique<utils::GpioVideoStatus>()} {
  // Init storage
  auto storage_result = storage_.Init(common::Config().GetString(kDBSection, kTargetOption, {}),
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
    return common::make_error_code(common::AteError::kInvalidDurationLongPress);
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

std::pair<cv::Rect, std::error_code> ATE::WaitForObject(const common::ObjectDataIdentity& object_data_identity,
                                                        const std::chrono::milliseconds& timeout) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;

  cv::Rect match_area{};
  std::error_code match_error{common::AteError::kPatternNotFound};
  cv::Mat pattern;

  auto objects = storage_.GetItemDataByWildcard(object_data_identity);
  for (const auto& object : objects) {
    pattern = storage_.GetItem(object.name);
    const bool is_image = !pattern.empty();
    do {
      std::tie(match_area, match_error) =
          is_image ? matcher_.MatchImage(object.name, pattern) : matcher_.MatchText(object.name);
    } while (match_error == common::AteError::kPatternNotFound && std::chrono::steady_clock::now() <= timeout_point);
  }

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

std::error_code ATE::ReloadStorageItems() noexcept {
  /* no-op */
  return {};
}

std::error_code ATE::GetScreenshot(const std::string& path, const std::string& filename) {
  return matcher_.GetScreenshot(path, filename);
}

std::pair<std::string, std::error_code> ATE::GetText(const cv::Point& point, const cv::Point& delta_point) {
  return matcher_.GetText(point, delta_point);
}

std::vector<common::ObjectData> ATE::GetObjectsDataByPattern(const common::ObjectDataIdentity& pattern) {
  return storage_.GetItemDataByWildcard(pattern);
}

std::pair<int, std::error_code> ATE::GetImagesDiscrepancy(const std::string& icon_path_second,
                                                          const std::string& icon_path_first,
                                                          const cv::Point& top_left_coordinate,
                                                          const cv::Point& bottom_right_coordinate) const {
  return matcher_.GetImagesDiscrepancy(icon_path_second, icon_path_first, top_left_coordinate, bottom_right_coordinate);
}

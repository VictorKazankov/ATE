#include "ate.h"

#include <stdexcept>
#include <utility>

#include <recognition/factory.h>

#include "ate_error.h"
#include "common.h"
#include "exceptions.h"
#include "factory/recognition_factory.h"
#include "interaction/SPI/spi_interaction.h"
#include "interaction/VDP/vdp_interaction.h"
#include "interaction/dummy/dummy_interaction.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "utils/env_config.h"
#include "utils/video_status.h"
#include "video_streaming/streamer_factory.h"

using namespace defines;

namespace {

DisplayType StrToDisplayType(const std::string& display_type_str) {
  if ("G1_8INCH_DISP" == display_type_str) return DisplayType::G1_8INCH_DISP;
  if ("G2_6INCH_DISP" == display_type_str) return DisplayType::G2_6INCH_DISP;
  if ("G2_8INCH_DISP" == display_type_str) return DisplayType::G2_8INCH_DISP;
  if ("G2_10LINCH_DISP" == display_type_str) return DisplayType::G2_10LINCH_DISP;
  if ("G2_10PINCH_DISP" == display_type_str) return DisplayType::G2_10PINCH_DISP;
  return DisplayType::UNDEFINED_DISP;
}

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

std::unique_ptr<interaction::Interaction> InteractionFactory(const std::string& interaction_type,
                                                             boost::asio::io_context& io_context) {
  if (interaction_type == kSpi) {
    return std::make_unique<interaction::SpiInteraction>(
        utils::GetEnvSettings(utils::EnvSettings::kSpiDeviceAddress),
        StrToDisplayType(common::Config().GetString(kInteraction, kDisplayTypeOption, {})));
  }

  if (interaction_type == kVdp) {
    return std::make_unique<interaction::VDPInteraction>(
        io_context, common::Config().GetString(kInteraction, kAddressOption, {}),
        common::Config().GetString(kInteraction, kPortOption, {}),
        StrToDisplayType(common::Config().GetString(kInteraction, kDisplayTypeOption, {})));
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

ATE::ATE(boost::asio::io_context& io_context)
    : interaction_{InteractionFactory(common::Config().GetString(kInteraction, kInteractionType, {}), io_context)},
      matcher_{streamer::MakeStreamer(),
               detector::MakeImageDetector(
                   StrToDetectorType(common::Config().GetString(kImageDetectorSection, kImageDetectorMatchingType,
                                                                kMultiscaleTemplateMatching)),
                   common::Config().GetDouble(kImageDetectorSection, kImageDetectorConfidenceOption, {})),
               factory::CreateTextDetector(common::Config().GetString(kDBSection, kTargetOption, {})),
               MakeScreenshotRecorder(), std::make_unique<utils::GpioVideoStatus>()} {
  // Init storage
  auto is_init = storage_.Init(VHAT_ICON_STORAGE_PREFIX, common::Config().GetString(kDBSection, kTargetOption, {}),
                               common::Config().GetString(kDBSection, kBuildOption, {}),
                               common::Config().GetString(kDBSection, kCollectionModeOption, {}));
  if (!is_init) {
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

std::pair<cv::Rect, std::error_code> ATE::WaitForObject(const std::string& object_or_name,
                                                        const std::chrono::milliseconds& timeout) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;
  auto pattern = storage_.GetItem(object_or_name);
  const bool is_image = !pattern.empty();
  cv::Rect match_area;
  std::error_code match_error;

  do {
    std::tie(match_area, match_error) =
        is_image ? matcher_.MatchImage(object_or_name, pattern) : matcher_.MatchText(object_or_name);
  } while (match_error == common::AteError::kPatternNotFound && std::chrono::steady_clock::now() <= timeout_point);

  return {match_area, match_error};
}

void ATE::ChangeResolution(int x, int y) { matcher_.ChangeResolution(x, y); }

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

std::error_code ATE::ReloadStorageItems() noexcept { return storage_.ReloadStorage(); }

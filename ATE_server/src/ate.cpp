#include "ate.h"

#include <stdexcept>
#include <utility>

#include <db_manager/factory.h>
#include <recognition/factory.h>

#include <opencv2/imgcodecs.hpp>

#include "common.h"
#include "exceptions.h"
#include "interaction/SPI/spi_interaction.h"
#include "interaction/VDP/vdp_interaction.h"
#include "interaction/dummy/dummy_interaction.h"
#include "logger/logger.h"
#include "utils/defines.h"
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
        common::Config().GetString(kInteraction, kDeviceAddress, {}),
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
                                                                kTemplateMathcing)),
                   common::Config().GetDouble(kImageDetectorSection, kImageDetectorConfidenceOption, {})),
               detector::MakeTextDetector(
                   common::Config().GetString(kTextDetectorSection, kTessDataOption, {}),
                   common::Config().GetString(kTextDetectorSection, kTextDetectorPreprocessingList, {}),
                   common::Config().GetDouble(kTextDetectorSection, kTextDetectorConfidenceOption,
                                              kDefaultTextDetectorConfidence)),
               MakeScreenshotRecorder()} {
  // create storage manager
  std::error_code error;
  storage_ = db_manager::CreateFileStorageManager(VHAT_SERVER_STORAGE_DIR "/icon_storage", error);
  if (error || storage_ == nullptr) {
    logger::error("[ATE] Can't create storage manager: {}.", error.message());
    throw storage::ConnectionFailure{};
  }
  error = storage_->InitCollectionsSettings(common::Config().GetString(kDBSection, kTargetOption, {}),
                                            common::Config().GetString(kDBSection, kBuildOption, {}),
                                            common::Config().GetString(kDBSection, kCollectionModeOption, {}));
  if (error) {
    logger::error("[ATE] Can't init storage manager: {}.", error.message());
    throw storage::ConnectionFailure{};
  }
}

ATE::~ATE() = default;

void ATE::TapObject(const cv::Point& point) { interaction_->Tap(point.x, point.y); }

void ATE::TouchAndDrag(const std::string& object_or_name, const cv::Point& start_point, const cv::Point& delta_point) {
  logger::trace("objectOrName '{}' has been draged from x: {} y: {} by dx: {} dy: {}", object_or_name, start_point.x,
                start_point.y, delta_point.x, delta_point.y);

  interaction_->TouchAndDrag(start_point.x, start_point.y, delta_point.x, delta_point.y);
}

cv::Rect ATE::WaitForObject(const std::string& object_or_name, const std::chrono::milliseconds& timeout) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;
  const auto item_info = storage_->GetItem(object_or_name, db_manager::kDefaultSyncVersion,
                                           db_manager::kDefaultSyncBuildVersion, db_manager::kDefaultCollectionMode);
  cv::Mat pattern{};
  if (!item_info.item.data.empty()) {
    pattern = cv::imdecode(cv::Mat(item_info.item.data), cv::IMREAD_COLOR);
  }
  const bool is_image = !item_info.error;
  cv::Rect match_result;

  do {
    match_result = is_image ? matcher_.MatchImage(object_or_name, pattern) : matcher_.MatchText(object_or_name);
  } while (match_result.empty() && std::chrono::steady_clock::now() <= timeout_point);

  return match_result;
}

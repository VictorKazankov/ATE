#include "ate.h"

#include <stdexcept>
#include <utility>

#include "common.h"
#include "exceptions.h"
#include "interaction/SPI/spi_interaction.h"
#include "interaction/VDP/vdp_interaction.h"
#include "interaction/dummy/dummy_interaction.h"
#include "logger/logger.h"
#include "storage/json_storage.h"
#include "utils/defines.h"
#include "video_streaming/matching/template_detector.h"
#include "video_streaming/matching/text_detector.h"
#include "video_streaming/matching/text_detector_decorator.h"
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

std::unique_ptr<detector::Detector<std::string>> MakeTextDetector() {
  const std::string tessdata_prefix = common::Config().GetString(kTextDetectorSection, kTessDataOption, {});
  if (tessdata_prefix.empty()) {
    logger::warn("[ATE] TESSDATA_PREFIX isn't present in config. Possible problems with text detection");
  }

  return std::make_unique<detector::TextDetectorDecorator>(
      std::make_unique<detector::TextDetector>(
          common::Config().GetDouble(kTextDetectorSection, kTextDetectorConfidenceOption,
                                     kDefaultTextDetectorConfidence),
          tessdata_prefix.c_str()),
      common::Config().GetString(kTextDetectorSection, kTextDetectorPreprocessingList, {}));
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
    : storage_{std::make_unique<storage::JsonStorage>(VHAT_SERVER_STORAGE_DIR "/icon_storage")},
      interaction_{InteractionFactory(common::Config().GetString(kInteraction, kInteractionType, {}), io_context)},
      matcher_{streamer::MakeStreamer(),
               std::make_unique<detector::TemplateDetector>(common::Config().GetDouble(
                   kImageDetectorSection, kImageDetectorConfidenceOption, kDefaultImageDetectorConfidence)),
               MakeTextDetector(), MakeScreenshotRecorder()} {
  if (!storage_->LoadCollection(common::Config().GetString(kDBSection, kTargetOption, {}),
                                common::Config().GetString(kDBSection, kBuildOption, {}),
                                common::Config().GetString(kDBSection, kCollectionModeOption, {}))) {
    throw storage::ConnectionFailure{};
  }
}

ATE::~ATE() = default;

void ATE::TapObject(const cv::Point& point) { interaction_->Tap(point.x, point.y); }

cv::Rect ATE::WaitForObject(const std::string& object_or_name, const std::chrono::milliseconds& timeout) {
  const auto timeout_point = std::chrono::steady_clock::now() + timeout;
  const auto item_path = storage_->ItemPath(object_or_name);
  const bool is_image = !item_path.empty();
  cv::Rect match_result;

  do {
    match_result = is_image ? matcher_.MatchImage(object_or_name, item_path) : matcher_.MatchText(object_or_name);
  } while (match_result.empty() && std::chrono::steady_clock::now() <= timeout_point);

  return match_result;
}

#ifndef ATE_SERVER_UTILS_DEFINES_H_
#define ATE_SERVER_UTILS_DEFINES_H_

namespace defines {

enum class ContactId { kOneFingerTouch = 0 };

constexpr auto kDBSection = "DB";
constexpr auto kTargetOption = "Target";
constexpr auto kBuildOption = "Build";
constexpr auto kCollectionModeOption = "CollectionMode";
constexpr auto kInteraction = "INTERACTION";
constexpr auto kInteractionType = "Type";
constexpr auto kDeviceAddress = "DeviceAddress";
constexpr auto kAteListenerPort = "ListenerPort";

constexpr auto kScreenshotRecorderSection = "SCREENSHOT_RECORDER";
constexpr auto kScreenshotOption = "Screenshots";
constexpr auto kScreenshotDirectoryOption = "ScreenshotDirectory";

constexpr auto kImageDetectorSection = "IMAGE_DETECTOR";
constexpr auto kImageDetectorConfidenceOption = "Confidence";
constexpr auto kImageDetectorMatchingType = "MatchingType";

constexpr auto kTextDetectorSection = "TEXT_DETECTOR";
constexpr auto kTextDetectorConfidenceOption = "Confidence";
constexpr double kDefaultTextDetectorConfidence = 0.;
constexpr auto kTessDataOption = "TESSDATA_PREFIX";
constexpr auto kTextDetectorPreprocessingList = "PreprocessingList";

constexpr auto kSpi = "SPI";
constexpr auto kDummy = "Dummy";

constexpr auto kTemplateMathcing = "TemplateMatching";
constexpr double kDefaultImageDetectorConfidence = 0.9;

constexpr auto kMultiscaleTemplateMatching = "MultiscaleTemplateMatching";
constexpr double kDefaultMultiscaleImageDetectorConfidence = 0.5;

const std::string kVideoStreamSection = "VIDEO_STREAM";

const std::string kVideoStreamSourceOption = "Source";
constexpr auto kVideoStreamTypeRtsp = "RTSP";
constexpr auto kVideoStreamTypeGstreamer = "GStreamer";
constexpr auto kVideoStreamTypeSyncVideo = "SyncVideo";

constexpr auto kVideoStreamPathOption = "Path";

constexpr auto kFrameWidthOption = "FrameWidth";
constexpr auto kFrameHeightOption = "FrameHeight";

const std::string kRtspAddressOption = "RtspStream";

constexpr auto kDBusInterface = "com.luxoft.vdp.prime.interface";
}  // namespace defines

#endif  // ATE_SERVER_UTILS_DEFINES_H_

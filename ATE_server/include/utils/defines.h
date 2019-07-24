#ifndef ATE_SERVER_UTILS_DEFINES_H_
#define ATE_SERVER_UTILS_DEFINES_H_

namespace defines {
enum class DisplayType {
  UNDEFINED_DISP = -1,
  G1_8INCH_DISP = 0,
  G2_6INCH_DISP = 1,
  G2_8INCH_DISP = 2,
  G2_10LINCH_DISP = 3,
  G2_10PINCH_DISP = 4
};

enum class ContactId { kOneFingerTouch = 0 };

constexpr auto kDBSection = "DB";
constexpr auto kTargetOption = "Target";
constexpr auto kCollectionModeOption = "CollectionMode";
constexpr auto kInteraction = "INTERACTION";
constexpr auto kAddressOption = "Address";
constexpr auto kPortOption = "Port";
constexpr auto kDisplayTypeOption = "DisplayType";
constexpr auto kInteractionType = "Type";
constexpr auto kDeviceAddress = "DeviceAddress";
constexpr auto kAteListenerPort = "ListenerPort";

constexpr auto kScreenshotRecorderSection = "SCREENSHOT_RECORDER";
constexpr auto kScreenshotOption = "Screenshots";
constexpr auto kScreenshotDirectoryOption = "ScreenshotDirectory";

constexpr auto kImageDetectorSection = "IMAGE_DETECTOR";
constexpr auto kImageDetectorConfidenceOption = "Confidence";
constexpr double kDefaultImageDetectorConfidence = 0.98;

constexpr auto kTextDetectorSection = "TEXT_DETECTOR";
constexpr auto kTextDetectorConfidenceOption = "Confidence";
constexpr double kDefaultTextDetectorConfidence = 0.;
constexpr auto kTessDataOption = "TESSDATA_PREFIX";

constexpr auto kVdp = "VDP";
constexpr auto kSpi = "SPI";
constexpr auto kDummy = "Dummy";

const std::string kVideoStreamSection = "VIDEO_STREAM";

const std::string kVideoStreamSourceOption = "Source";
constexpr auto kVideoStreamTypeV4l2 = "V4L2";
constexpr auto kVideoStreamTypeRtsp = "RTSP";
constexpr auto kVideoStreamTypeGstreamer = "GStreamer";

constexpr auto kVideoStreamPathOption = "Path";

constexpr auto kFrameWidthOption = "FrameWidth";
constexpr auto kFrameHeightOption = "FrameHeight";

const std::string kRtspAddressOption = "RtspStream";
}  // namespace defines

#endif  // ATE_SERVER_UTILS_DEFINES_H_

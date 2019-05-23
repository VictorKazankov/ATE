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

constexpr auto kVideoStreamOption = "VideoStream";
constexpr auto kDBSection = "DB";
constexpr auto kPathOption = "Path";
constexpr auto kCollectionModeOption = "CollectionMode";
constexpr auto kBoardSection = "BOARD";
constexpr auto kInteraction = "INTERACTION";
constexpr auto kAddressOption = "Address";
constexpr auto kPortOption = "Port";
constexpr auto kDisplayTypeOption = "DisplayType";
constexpr auto kInteractionType = "Type";
constexpr auto kDeviceAddress = "DeviceAddress";

constexpr auto kTextDetectorSection = "MATCHING/TextDetector";
constexpr auto kTextDetectorConfidenceOption = "Confidence";
constexpr double kDefaultTextDetectorConfidence = 80.;
constexpr auto kTessDataOption = "TESSDATA_PREFIX";

constexpr auto kVdp = "VDP";
constexpr auto kSpi = "SPI";

}  // namespace defines

#endif  // ATE_SERVER_UTILS_DEFINES_H_

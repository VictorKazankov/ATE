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

const std::string kVideoStreamOption = "VideoStream";
const std::string kDBSection = "DB";
const std::string kPathOption = "Path";
const std::string kCollectionModeOption = "CollectionMode";
const std::string kBoardSection = "BOARD";
const std::string kAddressOption = "Address";
const std::string kPortOption = "Port";
const std::string kDisplayTypeOption = "DisplayType";

constexpr auto kTextDetectorSection = "MATCHING/TextDetector";
constexpr auto kTextDetectorConfidenceOption = "Confidence";
constexpr double kDefaultTextDetectorConfidence = 80.;
constexpr auto kTessDataOption = "TESSDATA_PREFIX";

}  // namespace defines

#endif  // ATE_SERVER_UTILS_DEFINES_H_

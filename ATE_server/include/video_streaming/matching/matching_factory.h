#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHING_FACTORY_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHING_FACTORY_H_

#include <memory>

#include "utils/defines.h"
#include "video_streaming/matching/detector.h"

namespace detector {

/**
 * @brief MakeImageDetector factory function for constructin on image detectors
 * @params detector_type - type of detector (should be provided by config)
 * @params confidence - confidence level for recognition algorithm (default value for each algorithm is exist)
 * @return image detector based on "detector_type"
 */
std::unique_ptr<detector::Detector<cv::Mat>> MakeImageDetector(const std::string& detector_type,
                                                               double confidence = 0.);

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHING_FACTORY_H_
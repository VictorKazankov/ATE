#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_TEMPLATE_DETECTOR_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_TEMPLATE_DETECTOR_H_

#include <opencv2/core/mat.hpp>

#include "video_streaming/matching/detector.h"

namespace detector {

/**
 * @brief class TemplateDetector for detection of objects
 *  based on TM_CCOEFF_NORMED algorithm.
 **/
class TemplateDetector : public Detector {
 public:

/**
   * @brief Default constructor
   **/
  ~TemplateDetector() override = default;

  /**
   * @brief Detect an image pattern at a video frame
   * @param frame - video frame received from Sync (single channel matrix)
   * @param pattern - pattern to be detected (single channel matrix)
   * @return pattern coordinates on succeed, otherwise return empty Rect
   **/
  cv::Rect Detect(const cv::Mat& frame, const cv::Mat& pattern) const override;
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_TEMPLATE_DETECTOR_H_

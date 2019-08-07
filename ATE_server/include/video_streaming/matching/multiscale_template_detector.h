#ifndef ATE_SERVER_VIDEO_STREAMING_MULTISCALE_TEMPLATE_DETECTOR_H_
#define ATE_SERVER_VIDEO_STREAMING_MULTISCALE_TEMPLATE_DETECTOR_H_

#include <opencv2/core/mat.hpp>

#include "video_streaming/matching/detector.h"

namespace detector {

/**
 * @brief class MultiscaleTemplateDetector for detection of objects
 *  based on TM_CCORR_NORMED algorithm and rescaling and preprocessing
 *  of frame via Canny edge detector.
 **/
class MultiscaleTemplateDetector : public Detector<cv::Mat> {
 private:
  double confidence_;

 public:
  /**
   * @param confidence - confidence for recognition
   */
  explicit MultiscaleTemplateDetector(double confidence);

  /**
   * @brief Default destructor
   **/
  ~MultiscaleTemplateDetector() override = default;

  /**
   * @brief Detect an image pattern at a video frame
   * @param frame - video frame received from Sync (single channel matrix)
   * @param pattern - pattern to be detected (single channel matrix)
   * @return pattern coordinates on succeed, otherwise return empty Rect
   **/
  cv::Rect Detect(const cv::Mat& frame, const cv::Mat& pattern) override;
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MULTISCALE_TEMPLATE_DETECTOR_H_

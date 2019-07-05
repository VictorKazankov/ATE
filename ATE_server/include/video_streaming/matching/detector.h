#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_

#include <opencv2/core/types.hpp>

namespace detector {

/**
 * @brief Interface class for detectors
 **/
template <typename T>
class Detector {
 public:
  virtual ~Detector() = default;

  /**
   * @brief Detect an pattern at a video frame
   * @param frame video frame received from Sync
   * @param pattern a pattern to be detected
   * @return pattern coordinates
   **/
  virtual cv::Rect Detect(const cv::Mat& frame, const T& pattern) const = 0;
};
}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_

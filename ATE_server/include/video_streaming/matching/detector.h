#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_

#include <string>

namespace cv {
class Mat;
class Rect;
}  // namespace cv

namespace detector {

/**
 * @brief Interface class for detectors
 **/
class Detector {
 private:
  std::string name_;

 public:
  virtual ~Detector() = default;

  /**
   * @brief Detect an image pattern at a video frame
   * @param frame video frame received from Sync
   * @param pattern a pattern to be detected
   * @return pattern coordinates
   **/
  virtual cv::Rect Detect(const cv::Mat& frame, const cv::Mat& pattern) const = 0;

  /**
   * @brief Gets name of the detector
   * @return name of the detector
   **/
  const std::string& Name() const { return name_; }
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_

#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

#include <memory>

#include <opencv2/core/mat.hpp>

#include "detector.h"
#include "utils/screenshot_recorder.h"

namespace streamer {
class Streamer;
}  // namespace streamer

namespace detector {

/**
 * @brief DetectorFactory class for providing interaction between components: streamer, detector
 * @params detector_type string represantation of detector type, get from configuration file
 **/
class Matcher {
 private:
  std::unique_ptr<streamer::Streamer> streamer_;
  std::unique_ptr<Detector<cv::Mat>> image_detector_;
  std::unique_ptr<Detector<std::string>> text_detector_;
  std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder_;

  double text_detector_min_confidence_;

  // Cache images
  cv::Mat screen_;
  cv::Mat gray_screen_;

  bool GrabNewFrame();

 public:
  Matcher();
  ~Matcher();

  /**
   * @brief DetectImage privide detection of pattern on TDK screen
   * @params object object name
   * @params object_path path to pattern image
   * @return rectangle with x, y, width, height of detected object on succeed,
   * on failure x, y, width, height will equele 0
   */
  cv::Rect MatchImage(const std::string& object, const std::string& object_path);

  /**
   * @brief DetectImage privide detection of pattern on TDK screen
   * @params text text which need to find on TDK screen
   * @return rectangle with x, y, width, height of detected object on succeed,
   * on failure x, y, width, height will equele 0
   */
  cv::Rect MatchText(const std::string& text);
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

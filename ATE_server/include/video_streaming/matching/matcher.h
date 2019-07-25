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
 * @brief Matcher class for providing interaction between components: streamer, detector
 **/
class Matcher {
 private:
  std::unique_ptr<streamer::Streamer> streamer_;
  std::unique_ptr<Detector<cv::Mat>> image_detector_;
  std::unique_ptr<Detector<std::string>> text_detector_;
  std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder_;

  // Cache images
  cv::Mat screen_;
  cv::Mat gray_screen_;

  bool GrabNewFrame();

 public:
  /**
   * @params streamer - Streamer. Must be not null
   * @params image_detector - Image detector. Must be not null
   * @params text_detector - Text detector. Must be not null
   * @params screenshot_recorder - Screenshot recorder or null
   */
  Matcher(std::unique_ptr<streamer::Streamer> streamer, std::unique_ptr<Detector<cv::Mat>> image_detector,
          std::unique_ptr<Detector<std::string>> text_detector,
          std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder);

  ~Matcher();

  // disable copy and move
  Matcher(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  /**
   * @brief MatchImage provide image detection of the pattern on TDK screen
   * @params object object name
   * @params object_path path to pattern image
   * @return rectangle with x, y, width, height of the detected object on succeed,
   * on failure x, y, width, height will equal 0
   */
  cv::Rect MatchImage(const std::string& object, const std::string& object_path);

  /**
   * @brief MatchText provide text detection of the pattern on TDK screen
   * @params text text which need to find on TDK screen
   * @return rectangle with x, y, width, height of the detected object on succeed,
   * on failure x, y, width, height will equal 0
   */
  cv::Rect MatchText(const std::string& text);
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

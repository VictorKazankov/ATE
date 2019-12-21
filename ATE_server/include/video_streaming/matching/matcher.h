#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

#include <memory>

#include <opencv2/core/mat.hpp>

#include <recognition/detector.h>

#include "utils/screenshot_recorder.h"

namespace streamer {
class Streamer;
}  // namespace streamer

namespace utils {
class VideoStatus;
}  // namespace utils

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
  std::unique_ptr<utils::VideoStatus> video_status_;

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
          std::unique_ptr<utils::ScreenshotRecorder> screenshot_recorder,
          std::unique_ptr<utils::VideoStatus> video_status);

  ~Matcher();

  // disable copy and move
  Matcher(const Matcher&) = delete;
  Matcher(Matcher&&) = delete;
  Matcher& operator=(const Matcher&) = delete;
  Matcher& operator=(Matcher&&) = delete;

  /**
   * @brief MatchImage provide image detection of the pattern on TDK screen
   * @param object - object name
   * @param pattern - pattern image
   * @return rectangle with x, y, width, height of the detected object on succeed,
   * on failure return error code and empty rectangle
   */
  std::pair<cv::Rect, std::error_code> MatchImage(const std::string& object, const cv::Mat& pattern);

  /**
   * @brief MatchText provide text detection of the pattern on TDK screen
   * @param text - text which need to find on TDK screen
   * @return rectangle with x, y, width, height of the detected object on succeed,
   * on failure return error code and empty rectangle
   */
  std::pair<cv::Rect, std::error_code> MatchText(const std::string& text);

  /**
   * @brief The function changes resolution for video stream
   * @param x - a new high value
   * @param y - a new width value
   */
  void ChangeResolution(int x, int y);

  /**
   * @brief Change preprocessing list for text detector
   * @param sync_version Sync version
   */
  void ChangePreprocessingList(const std::string& sync_version);

};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

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
   * @brief GetFrame grabs frame from stream
   * @param frame Output frame from stream
   * @param area Desired area
   * @return Error if an error occured while reading frame
   */
  std::error_code GetFrame(cv::Mat& frame, const cv::Rect& area = cv::Rect());

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

  /**
   * @brief The function gets screenshot from the current screen
   * @param file_name Name of screenshot file
   * @param file_path Location for saving screenshot with prefix 'screenshots_store_dir_'
   * @param area Desiread area
   * @return Error code in case screenshot failed, otherwise empty error_code
   */
  std::error_code GetScreenshot(const std::string& file_name, const std::string& file_path,
                                const cv::Rect& area = cv::Rect());

  /**
   * @brief The function extracts text from predefined area in accordance with coordinates
   * @param point of start of predefined rectangle
   * @param delta_point of end of predefined rectangle
   * @return string representation of detected text on predefined area on succeed,
   * on failure return error code and empty string
   */
  std::pair<std::string, std::error_code> GetText(const cv::Point& point, const cv::Point& delta_point);

  /**
   * @brief Gets the difference between the two images as a percentage
   * @param icon_path_second Path to the second image
   * @param icon_path_first Path to the first image
   * @param top_left_coordinate Top left  coordinate
   * @param bottom_right_coordinate Bottom right coordinate
   * @return A discrepancy coefficient and error code of operation
   */
  std::pair<int, std::error_code> GetImagesDiscrepancy(const std::string& icon_path_second,
                                                       const std::string& icon_path_first,
                                                       const cv::Point& top_left_coordinate,
                                                       const cv::Point& bottom_right_coordinate) const;
};

}  // namespace detector

#endif  // ATE_SERVER_VIDEO_STREAMING_MATCHING_MATCHER_H_

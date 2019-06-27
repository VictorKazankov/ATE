#ifndef ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_
#define ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_

#include <opencv2/core/types.hpp>

#include <experimental/filesystem>

namespace utils {

namespace fs = std::experimental::filesystem;

/**
 * @brief Class for recording screenshots
 **/
class ScreenshotRecorder {
  bool storage_directory_available_ = false;
  bool enable_saving_screenshots_ = false;
  fs::path screenshots_store_dir_;

 public:
  /**
   * @brief Constructor
   * @param enable_saving_screenshots flag which turns on creation of screenshots while recognition
   * @param screenshots_store_dir points out folder to store screenshots
   **/
  ScreenshotRecorder(bool enable_saving_screenshots, const std::string& screenshots_store_dir);

  /**
   * @brief Function returns flag whether screenshots allowed to store
   * @return true if allowed, otherwise - false
   */
  bool IsScreenshotSavingEnabled() const;

  /**
   * @brief Saves screenshot to file
   * @param frame frame to save to file
   * @return true if successful, otherwise - false
   **/
  bool SaveScreenshot(const cv::Mat& frame) const;

  /**
   * @brief Saves screenshot to file with highlight area
   * @param frame frame to save to file
   * @param highlight_area area to highlight at the frame
   * @param hint suffix for screenshot name
   * @return true if successful, otherwise - false
   **/
  bool SaveScreenshot(const cv::Mat& frame, const cv::Rect& highlight_area, const std::string& hint = "") const;

  /**
   * @brief Saves screenshot to file with and without highlight area.
   * It's a wrapper for SaveScreenshot(const cv::Mat&) and SaveScreenshot(const cv::Mat&, const cv::Rect&, const std::string&)
   * @param color_frame frame to save to file without highlight area
   * @param grey_frame frame to save to file with highlight area
   * @param highlight_area area to highlight at the frame
   * @param hint suffix for screenshot name
   * @return true if successful, otherwise - false
   **/
  bool TakeScreenshots(const cv::Mat& color_frame, const cv::Mat& grey_frame, const cv::Rect& highlight_area, const std::string& hint = "") const;

 private:
  void ProcessStorageDirectory();
  fs::path GetFileName(const std::string& file_suffix = "") const;
};
}  // namespace utils

#endif  // ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_

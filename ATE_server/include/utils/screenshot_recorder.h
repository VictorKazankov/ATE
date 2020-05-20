#ifndef ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_
#define ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_

#include <opencv2/core/types.hpp>

#include <experimental/filesystem>

namespace utils {

namespace fs = std::experimental::filesystem;

/**
 * @brief Screenshot error code. Used to determine the result of function GetScreenshot.
 */
enum class ScreenshotError {
  kEmptyFileName,
  kPermissionDenied,
  kWrongExtension,
  kSystemError,
  kImageAssemblingFailed,
  kNoAvailableDiskSpace,
  kSuccess
};

/**
 * @brief Class for recording screenshots
 **/
class ScreenshotRecorder {
  bool enable_saving_screenshots_ = false;
  fs::path screenshots_store_dir_;

 public:
  /**
   * @brief Constructor
   * @param enable_saving_screenshots Flag which turns on creation of screenshots while recognition
   * @param screenshots_store_dir Points out folder to store screenshots
   **/
  ScreenshotRecorder(bool enable_saving_screenshots, const std::string& screenshots_store_dir);

  /**
   * @brief Saves screenshot to file
   * @param frame Frame to save to file
   * @param filename Filename (short name)
   * @return True if successful, otherwise - false
   **/
  bool SaveScreenshot(const cv::Mat& frame, const std::string& filename = "") const;

  /**
   * @brief Saves screenshot to file with highlight area
   * @param frame Frame to save to file
   * @param highlight_areas Areas to highlight at the frame
   * @param hint Suffix for screenshot name
   * @return True if successful, otherwise - false
   **/
  bool SaveScreenshot(const cv::Mat& frame, const std::vector<cv::Rect>& highlight_areas,
                      const std::string& hint = "") const;

  /**
   * @brief Saves screenshot to file with and without highlight area.
   * It's a wrapper for SaveScreenshot(const cv::Mat&) and
   * SaveScreenshot(const cv::Mat&, const cv::Rect&, const std::string&)
   * @param color_frame Frame to save to file without highlight area
   * @param grey_frame Frame to save to file with highlight area
   * @param highlight_areas Areas to highlight at the frame
   * @param hint Suffix for screenshot name
   * @return True if successful, otherwise - false
   **/
  bool TakeScreenshots(const cv::Mat& color_frame, const cv::Mat& grey_frame,
                       const std::vector<cv::Rect>& highlight_areas, const std::string& hint = "") const;

  /**
   * @brief Saves screenshot to file
   * @param color_frame Frame to save to file without highlight area
   * @param path Location for saving screenshot with prefix 'screenshots_store_dir_
   * @param file_name Name of screenshot file
   * @return True if screenshot saved, otherwise - false
   **/
  static ScreenshotError GetScreenshot(const cv::Mat& color_frame, const std::string& path,
                                       const std::string& file_name);

  /**
   * @brief Creates directory from path
   * @param path Full path to directory
   * @return Error whether failed to create directory, otherwise empty error
   **/
  static std::error_code MakeDirectories(const fs::path& dir);

 private:
  fs::path GetFileName(const std::string& file_suffix = "") const;
};
}  // namespace utils

#endif  // ATE_SERVER_UTILS_SCREENSHOT_RECORDER_H_

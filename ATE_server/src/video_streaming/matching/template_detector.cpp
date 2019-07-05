#include "video_streaming/matching/template_detector.h"

#include <cassert>

#include <opencv2/imgproc.hpp>

namespace detector {

TemplateDetector::TemplateDetector(double confidence) : confidence_{confidence} {}

cv::Rect TemplateDetector::Detect(const cv::Mat& frame, const cv::Mat& pattern) {
  // verification of size compatibility (pattern should be less than frame)
  assert(frame.rows > pattern.rows);
  assert(frame.cols > pattern.cols);

  // Perform the size of result matrix
  // Magic number 1 needs for ability to matching all possible locations for it.
  const auto rows = frame.rows - pattern.rows + 1;
  const auto cols = frame.cols - pattern.cols + 1;

  // The result of comparation between frame and pattern, must be
  // single-channel 32-bit floating-point.
  // If the frame is W×H and pattern is w×h, then the result is (W−w+1)×(H−h+1).
  cv::Mat value(rows, cols, CV_32FC1);
  cv::matchTemplate(frame, pattern, value, cv::TM_CCOEFF_NORMED);

  double minVal;
  double maxVal;
  cv::Point minLoc;
  cv::Point matchLoc;

  cv::minMaxLoc(value, &minVal, &maxVal, &minLoc, &matchLoc);

  // If template has not detected return empty Rect
  if (confidence_ > maxVal) {
    return cv::Rect();
  }
  return cv::Rect{matchLoc, cv::Size{pattern.cols, pattern.rows}};
}

}  // namespace detector

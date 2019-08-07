#include "video_streaming/matching/multiscale_template_detector.h"

#include <cassert>
#include <vector>

#include <opencv2/imgproc.hpp>

namespace {

template <typename T>
std::vector<T> linspace(T start, T stop, size_t length) {
  T h = (stop - start) / static_cast<T>(length - 1);
  std::vector<T> xs(length);
  typename std::vector<T>::iterator it;
  T step;

  for (it = xs.begin(), step = start; it != xs.end(); ++it, step += h) *it = step;
  return xs;
}

void Rescale(const cv::Mat& src, cv::Mat& resized, int width) {
  const int rows = src.rows;
  const int cols = src.cols;
  double scale = static_cast<double>(width) / cols;

  cv::Size dim{width, static_cast<int>(rows * scale)};

  cv::resize(src, resized, dim);
}

}  // namespace

detector::MultiscaleTemplateDetector::MultiscaleTemplateDetector(double confidence) : confidence_(confidence) {}

cv::Rect detector::MultiscaleTemplateDetector::Detect(const cv::Mat& frame, const cv::Mat& pattern) {
  // verification of size compatibility (pattern should be less than frame)
  assert(frame.rows > pattern.rows);
  assert(frame.cols > pattern.cols);

  // Perform the size of result matrix
  // Magic number 1 needs for ability to matching all possible locations for it.
  const auto rows = frame.rows - pattern.rows + 1;
  const auto cols = frame.cols - pattern.cols + 1;

  // Threshold value for TM_CCORR_NORMED
  double max_threshold{};
  double min_val;
  double max_val;

  cv::Point min_loc;
  cv::Point match_loc;

  cv::Rect rect{};

  cv::Mat pattern_edged;
  cv::Mat frame_edged;
  cv::Mat pattern_blured;
  cv::Mat frame_blured;
  cv::Mat rescaled_frame;
  cv::Mat matching_value(rows, cols, CV_32FC1);

  // bluring of edges for more accurate detection of edges
  cv::blur(pattern, pattern_blured, cv::Size{3, 3});
  cv::blur(frame, frame_blured, cv::Size{3, 3});

  // 50 - threshold for the hysteresis procedure
  // 200 - threshold for the hysteresis procedure
  // get edged representation of pattern
  cv::Canny(pattern_blured, pattern_edged, 50, 200);

  // get linearly spaced vector
  const auto linrange = linspace(0.1, 1.5, 15);

  for (const auto& scale : linrange) {
    // do rescale
    Rescale(frame_blured, rescaled_frame, static_cast<int>(frame.cols * scale));

    if (rescaled_frame.rows < pattern.rows || rescaled_frame.cols < pattern.cols) {
      continue;
    }

    // get rescaled and edged representation of the frame
    cv::Canny(rescaled_frame, frame_edged, 50, 200);

    cv::matchTemplate(frame_edged, pattern_edged, matching_value, cv::TM_CCORR_NORMED);
    cv::minMaxLoc(matching_value, &min_val, &max_val, &min_loc, &match_loc);

    // verify threshold
    if (confidence_ < max_val && max_val > max_threshold) {
      max_threshold = max_val;
      rect = cv::Rect{match_loc / scale,
                      cv::Size{static_cast<int>(pattern.cols / scale), static_cast<int>(pattern.rows / scale)}};
    }
  }

  return rect;
}

#include "video_streaming/framebuf_to_mat.h"

#include <opencv2/imgproc.hpp>

namespace streamer {

cv::Mat MakeMat(void* from_framebuf, int frame_width, int frame_height) {
  assert(from_framebuf != nullptr);

  cv::Mat result{frame_height, frame_width, CV_8UC4, from_framebuf};
  cv::cvtColor(result, result, cv::COLOR_RGBA2BGR);

  return result;
}

}  // namespace streamer

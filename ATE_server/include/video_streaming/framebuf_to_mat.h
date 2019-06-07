#ifndef ATE_SERVER_STREAMER_UTILS_FB_TO_MAT_H_
#define ATE_SERVER_STREAMER_UTILS_FB_TO_MAT_H_

#include <memory>

#include <opencv2/core/mat.hpp>

namespace streamer {

cv::Mat MakeMat(void* from_framebuf, int frame_width, int frame_height);

}  // namespace streamer

#endif  // ATE_SERVER_STREAMER_UTILS_FB_TO_MAT_H_

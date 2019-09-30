#ifndef ATE_SERVER_VIDEO_STREAMING_STREAMING_H_
#define ATE_SERVER_VIDEO_STREAMING_STREAMING_H_

#include <opencv2/core/mat.hpp>

namespace streamer {

/**
 * @brief Interface class for streamers
 **/
class Streamer {
 public:
  virtual ~Streamer() = default;

  /**
   * @brief The method gets video frame
   * @params reference to matrix (simple opencv matrix cv::Mat), made for increase performance.
   * @return 'true' in case of the frame has been read successfully otherwise false.
   **/
  virtual bool Frame(cv::Mat&) = 0;

  /**
   * @brief The method change resolution of video stream
   * @params x width of the frame
   * @params y height of the ftame
   **/
  virtual void ChangeResolution(int x, int y) = 0;
};

}  // namespace streamer

#endif  // ATE_SERVER_VIDEO_STREAMING_STREAMING_H_

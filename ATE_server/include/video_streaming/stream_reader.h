#ifndef ATE_SERVER_STREAM_READER_H_
#define ATE_SERVER_STREAM_READER_H_

#include <string>

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "video_streaming/streamer.h"

namespace streamer {

/**
 * @brief Class for capturing video stream via OpenCV
 **/
class StreamReader : public Streamer {
 private:
  cv::VideoCapture capture_;

 public:
  /**
   * @brief Default constructor of StreamReader Open video file or a capturing device or a IP video stream
   * @params filename it can be:
   * - name of video file (eg. `video.avi`)
   * - or image sequence (eg. `img_%02d.jpg`, which will read samples like `img_00.jpg, img_01.jpg, img_02.jpg, ...`)
   * - or URL of video stream (eg. `protocol://host:port/script_name?script_params|auth`).
   */
  explicit StreamReader(const std::string& source);
  ~StreamReader() override = default;

  StreamReader(const StreamReader&) = delete;
  StreamReader& operator=(const StreamReader&) = delete;

  /**
   * @brief The method pulls out a frame from a video stream.
   * @params reference to matrix (simple opencv matrix cv::Mat), made for increase performance.
   * @return 'true' in case of the frame has been read successfully otherwise false.
   */
  bool Frame(cv::Mat& frame) override;
};

}  // namespace streamer

#endif  // ATE_SERVER_STREAM_READER_H_

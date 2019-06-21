#ifndef ATE_SERVER_VIDEO_STREAMING_GST_STREAMER_H_
#define ATE_SERVER_VIDEO_STREAMING_GST_STREAMER_H_

#include "video_streaming/streamer.h"

#include <memory>

#include <gst/gst.h>

#include <opencv2/core/types.hpp>

namespace streamer {

class GstStreamer : public Streamer {
 public:
  GstStreamer(const std::string& path, const cv::Size& size);
  ~GstStreamer() override = default;

  /**
   * @brief The method gets video frame
   * @params reference to matrix (simple OpenCV matrix cv::Mat), made for increase performance.
   * @return 'true' in case of the frame has been read successfully, otherwise false.
   */
  bool Frame(cv::Mat& frame) override;

 private:
  struct GstElementDeleter {
    void operator()(GstElement* pipeline) const noexcept;
  };

  bool WriteGstBufferToCvMat(GstBuffer* buffer, cv::Mat& result) const;

  cv::Size matrix_size_;
  std::unique_ptr<GstElement, GstElementDeleter> pipeline_;
};

}  // namespace streamer

#endif  // ATE_SERVER_VIDEO_STREAMING_GST_STREAMER_H_

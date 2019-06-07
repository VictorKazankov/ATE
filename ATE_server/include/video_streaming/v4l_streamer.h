#ifndef ATE_SERVER_STREAMER_VIDEO_FOR_LINUX__H_
#define ATE_SERVER_STREAMER_VIDEO_FOR_LINUX__H_

#include <linux/videodev2.h>
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <thread>

#include "utils/file_descriptor.h"
#include "video_streaming/stream_reader.h"

namespace streamer {

class Video4Linux : public Streamer {
 public:
  ~Video4Linux() override;
  Video4Linux(const std::string& device, int frame_width, int frame_height);

  bool Frame(cv::Mat& frame) override;

 private:
  using DevBufferPtr = std::unique_ptr<void, std::function<void(void*)>>;
  using FrameBufferPtr = std::unique_ptr<std::uint8_t[]>;

  enum class DeviceRequest : unsigned long {
    kSetFormat = VIDIOC_S_FMT,
    kRequestBuffer = VIDIOC_REQBUFS,
    kQueryBuffer = VIDIOC_QUERYBUF,
    kEnqueBuffer = VIDIOC_QBUF,
    kEnableStream = VIDIOC_STREAMON,
    kDisableStream = VIDIOC_STREAMOFF,
    kDequeueBuffer = VIDIOC_DQBUF,
  };

  bool Ioctl(DeviceRequest request, void* data) const;

  bool SetFormat() const;
  bool RequestBuffer() const;
  bool QueryBuffer(v4l2_buffer& out_buffer) const;
  bool MapDeviceBuffer(v4l2_buffer& buffer);

  bool EnqueBuffer(v4l2_buffer& out_buffer) const;
  bool DequeBuffer(v4l2_buffer& buffer) const;
  bool WaitForFrame() const;

  bool StartCapture() const;
  bool StopCapture() const;
  bool CaptureFrame();
  void CaptureLoop();

  int frame_width_;
  int frame_height_;
  int frame_size_;
  utils::FileDescriptor device_;
  DevBufferPtr device_buffer_;
  FrameBufferPtr frame_buffer_;
  std::atomic_bool is_capturing_{false};
  std::thread capture_thread_;
};

}  // namespace streamer

#endif  // ATE_SERVER_STREAMER_VIDEO_FOR_LINUX__H_

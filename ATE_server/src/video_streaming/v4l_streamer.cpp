#include "video_streaming/v4l_streamer.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <system_error>
#include <utility>

#include "exceptions.h"
#include "logger/logger.h"
#include "video_streaming/framebuf_to_mat.h"

namespace streamer {

namespace {
const std::uint32_t kBufferType = V4L2_BUF_TYPE_VIDEO_CAPTURE;
const std::uint32_t kMemoryType = V4L2_MEMORY_MMAP;

const std::uint32_t kBufferCount = 1;
const std::uint32_t kBufferIndex = 0;

const int kBytesPerPixel = 4;
const int kFrameWaitTimeoutSeconds = 2;
}  // namespace

bool Video4Linux::Ioctl(DeviceRequest request, void* data) const {
  /* This is a normal practice to dumbly retry syscall if EINTR was returned
     (http://blog.reverberate.org/2011/04/eintr-and-pc-loser-ing-is-better-case.html)
     Manual handling of what signal we have received is a bit complicated. Consider switching to boost::asio. */
  do
    if (::ioctl(device_.Handle(), static_cast<unsigned long>(request), data) == 0) return true;
  while (std::error_code{errno, std::generic_category()} == std::errc::interrupted);

  return false;
}

bool Video4Linux::SetFormat() const {
  v4l2_format fmt{};
  fmt.type = kBufferType;
  fmt.fmt.pix.width = frame_width_;
  fmt.fmt.pix.height = frame_height_;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;

  return Ioctl(DeviceRequest::kSetFormat, &fmt);
}

bool Video4Linux::RequestBuffer() const {
  v4l2_requestbuffers request_buffer{};

  request_buffer.count = kBufferCount;
  request_buffer.type = kBufferType;
  request_buffer.memory = kMemoryType;

  return Ioctl(DeviceRequest::kRequestBuffer, &request_buffer);
}

bool Video4Linux::QueryBuffer(v4l2_buffer& out_buffer) const {
  out_buffer = {};
  out_buffer.type = kBufferType;
  out_buffer.memory = kMemoryType;
  out_buffer.index = kBufferIndex;

  return Ioctl(DeviceRequest::kQueryBuffer, &out_buffer);
}

bool Video4Linux::MapDeviceBuffer(v4l2_buffer& buffer) {
  const size_t length = buffer.length;

  auto unmap = [length](void* addr) {
    if (addr && addr != MAP_FAILED) {
      if (::munmap(addr, length) != 0) logger::error("[streamer][v4l] munmap() returned error: {}", errno);
    }
  };

  DevBufferPtr ptr{nullptr, unmap};
  void* const addr = ::mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, device_.Handle(), buffer.m.offset);

  if (addr != MAP_FAILED) {
    ptr.reset(addr);
    device_buffer_ = std::move(ptr);
    return true;
  }

  return false;
}

bool Video4Linux::EnqueBuffer(v4l2_buffer& out_buffer) const {
  out_buffer.type = kBufferType;
  out_buffer.memory = kMemoryType;
  out_buffer.index = kBufferIndex;

  return Ioctl(DeviceRequest::kEnqueBuffer, &out_buffer);
}

bool Video4Linux::DequeBuffer(v4l2_buffer& buffer) const { return Ioctl(DeviceRequest::kDequeueBuffer, &buffer); }

bool Video4Linux::WaitForFrame() const {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(device_.Handle(), &fds);
  timeval tv{};
  tv.tv_sec = kFrameWaitTimeoutSeconds;
  tv.tv_usec = 0;

  return ::select(device_.Handle() + 1, &fds, nullptr, nullptr, &tv) != -1;
}

bool Video4Linux::StartCapture() const {
  v4l2_buffer buffer;
  buffer.type = kBufferType;
  return Ioctl(DeviceRequest::kEnableStream, &buffer.type);
}

bool Video4Linux::StopCapture() const {
  v4l2_buffer buffer;
  buffer.type = kBufferType;
  return Ioctl(DeviceRequest::kDisableStream, &buffer.type);
}

bool Video4Linux::CaptureFrame() {
  v4l2_buffer buffer{};
  if (!EnqueBuffer(buffer)) {
    logger::error("[streamer][v4l] Failed to enque buffer");
    return false;
  }

  if (!WaitForFrame()) {
    logger::error("[streamer][v4l] Wait frame timeout (timeout is {} sec)", kFrameWaitTimeoutSeconds);
    return false;
  }

  if (!DequeBuffer(buffer)) {
    logger::error("[streamer][v4l] Failed to deque buffer");
    return false;
  }

  std::memcpy(frame_buffer_.get(), device_buffer_.get(), frame_size_);
  return true;
}

void Video4Linux::CaptureLoop() {
  while (is_capturing_) {
    if (!CaptureFrame()) is_capturing_ = false;
  }
}

bool Video4Linux::Frame(cv::Mat& frame) {
  if (is_capturing_) {
    frame = MakeMat(frame_buffer_.get(), frame_width_, frame_height_);
    return true;
  } else {
    return false;
  }
}

void Video4Linux::ChangeResolution(int x, int y) {
  assert(!"ChangeResolution feature not been tested for Video4Linux");
  frame_width_ = x;
  frame_height_ = y;
  SetFormat();
}

Video4Linux::~Video4Linux() {
  is_capturing_ = false;
  capture_thread_.join();
  if (!StopCapture()) logger::error("[streamer][v4l] Failed to stop video capture");
}

Video4Linux::Video4Linux(const std::string& device, int frame_width, int frame_height)
    : frame_width_{frame_width},
      frame_height_{frame_height},
      frame_size_{frame_width * frame_height * kBytesPerPixel},
      device_{device, O_RDWR | O_SYNC} {
  assert(frame_width_ >= 0);
  assert(frame_height_ >= 0);
  assert(frame_size_ >= 0);

  if (!SetFormat()) throw StreamOpenFailure{"Failed to set format"};
  if (!RequestBuffer()) throw StreamOpenFailure{"Failed to request V4L buffer"};

  v4l2_buffer buffer{};
  if (!QueryBuffer(buffer)) throw StreamOpenFailure{"Failed to query V4L buffer"};
  if (!(MapDeviceBuffer(buffer))) throw StreamOpenFailure{"Failed to map V4L buffer"};

  frame_buffer_.reset(new std::uint8_t[frame_size_]());

  assert(kBufferCount == 1);
  if (!StartCapture()) throw StreamOpenFailure{"Failed to start video capture"};
  if (!CaptureFrame()) throw StreamOpenFailure{"Failed to capture first frame"};
  if (!CaptureFrame()) throw StreamOpenFailure{"Failed to drain second frame"};

  is_capturing_ = true;
  capture_thread_ = std::thread{[this]() { CaptureLoop(); }};
}

}  // namespace streamer

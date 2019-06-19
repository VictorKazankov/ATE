#include "video_streaming/streamer_factory.h"

#include <stdexcept>
#include <string>

#include "common.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "video_streaming/gst_streamer.h"
#include "video_streaming/stream_reader.h"
#include "video_streaming/v4l_streamer.h"

namespace streamer {

namespace {
using namespace defines;

enum class VideoStreamType { kRtsp, kV4l2, kGstreamer };

VideoStreamType StreamType() {
  const std::string stream_type = common::Config().GetString(kVideoStreamSection, kVideoStreamSourceOption, {});

  if (stream_type == kVideoStreamTypeV4l2) {
    return VideoStreamType::kV4l2;
  } else if (stream_type == kVideoStreamTypeRtsp) {
    return VideoStreamType::kRtsp;
  } else if (stream_type == kVideoStreamTypeGstreamer) {
    return VideoStreamType::kGstreamer;
  } else {
    logger::critical("[video streamer] Invalid video stream type in config. Read value: {}", stream_type);
    throw InvalidConfig{};
  }
}

cv::Size GetFrameSize() {
  cv::Size frame_size{common::Config().GetInt(kVideoStreamSection, kFrameWidthOption, {}),
                      common::Config().GetInt(kVideoStreamSection, kFrameHeightOption, {})};

  if (frame_size.empty()) {
    throw InvalidConfig{};
  }

  return frame_size;
}

std::unique_ptr<Streamer> MakeV4l2Streamer(const std::string& device) {
  const auto frame_size = GetFrameSize();

  try {
    return std::make_unique<Video4Linux>(device, frame_size.width, frame_size.height);
  } catch (const streamer::StreamOpenFailure& v4l_exception) {
    logger::critical("[streamer] Failed to open V4L2 stream from device {} with frame {}x{}: {}", device,
                     frame_size.width, frame_size.height, v4l_exception.what());
    throw;
  }
}

}  // namespace

std::unique_ptr<Streamer> MakeStreamer(const std::string& path) {
  if (path.empty()) {
    throw InvalidConfig{};
  }
  switch (StreamType()) {
    case VideoStreamType::kV4l2:
      return MakeV4l2Streamer(path);
    case VideoStreamType::kRtsp:
      return std::make_unique<StreamReader>(path);
    case VideoStreamType::kGstreamer:
      return std::make_unique<GstStreamer>(path, GetFrameSize());
  }

  throw std::logic_error{"Incorrect Config parsing logic"};
}

}  // namespace streamer

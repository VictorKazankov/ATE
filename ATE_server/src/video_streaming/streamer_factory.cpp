#include "video_streaming/streamer_factory.h"

#include <stdexcept>
#include <string>

#include "common.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "video_streaming/stream_reader.h"
#include "video_streaming/v4l_streamer.h"

namespace streamer {

namespace {
using defines::kVideoStreamSection;

enum class VideoStreamType {
  kRtsp,
  kV4l2,
};

VideoStreamType StreamType() {
  using defines::kVideoStreamSourceOption;
  using defines::kVideoStreamTypeRtsp;
  using defines::kVideoStreamTypeV4l2;

  const std::string stream_type = common::Config().GetString(kVideoStreamSection, kVideoStreamSourceOption, {});

  if (stream_type == kVideoStreamTypeV4l2) {
    return VideoStreamType::kV4l2;
  } else if (stream_type == kVideoStreamTypeRtsp) {
    return VideoStreamType::kRtsp;
  } else {
    logger::critical("[video streamer] Invalid video stream type in config. Read value: {}", stream_type);
    throw InvalidConfig{};
  }
}

std::unique_ptr<Streamer> MakeV4l2Streamer() {
  using defines::kVideo4LinuxDeviceOption;
  using defines::kVideo4LinuxFrameHeightOption;
  using defines::kVideo4LinuxFrameWidthOption;

  const std::string device = common::Config().GetString(kVideoStreamSection, kVideo4LinuxDeviceOption, {});
  const int frame_width = common::Config().GetInt(kVideoStreamSection, kVideo4LinuxFrameWidthOption, {});
  const int frame_height = common::Config().GetInt(kVideoStreamSection, kVideo4LinuxFrameHeightOption, {});

  if (device.empty() || frame_width <= 0 || frame_height <= 0) throw InvalidConfig{};

  try {
    return std::make_unique<Video4Linux>(device, frame_width, frame_height);
  } catch (const streamer::StreamOpenFailure& v4l_exception) {
    logger::critical("[streamer] Failed to open V4L2 stream from device {} with frame {}x{}: {}", device, frame_width,
                     frame_height, v4l_exception.what());
    throw;
  }
}

std::unique_ptr<Streamer> MakeRtspStreamer() {
  using defines::kRtspAddressOption;

  const std::string rtsp_address = common::Config().GetString(kVideoStreamSection, kRtspAddressOption, {});
  if (rtsp_address.empty()) throw InvalidConfig{};

  return std::make_unique<StreamReader>(rtsp_address);
}

}  // namespace

std::unique_ptr<Streamer> MakeStreamer() {
  switch (StreamType()) {
    case VideoStreamType::kV4l2:
      return MakeV4l2Streamer();
    case VideoStreamType::kRtsp:
      return MakeRtspStreamer();
  }

  throw std::logic_error{"Incorrect Config parsing logic"};
}

}  // namespace streamer

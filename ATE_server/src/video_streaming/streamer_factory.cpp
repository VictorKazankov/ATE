#include "video_streaming/streamer_factory.h"

#include <stdexcept>
#include <string>

#include "common.h"
#include "exceptions.h"
#include "logger/logger.h"
#include "utils/defines.h"
#include "video_streaming/gst_streamer.h"
#include "video_streaming/stream_reader.h"
#include "video_streaming/sync_video_streamer.h"

namespace streamer {

namespace {
using namespace defines;

enum class VideoStreamType { kRtsp, kGstreamer, kSyncVideo };

VideoStreamType StreamType() {
  const std::string stream_type = common::Config().GetString(kVideoStreamSection, kVideoStreamSourceOption, {});

  if (stream_type == kVideoStreamTypeRtsp) {
    return VideoStreamType::kRtsp;
  } else if (stream_type == kVideoStreamTypeGstreamer) {
    return VideoStreamType::kGstreamer;
  } else if (stream_type == kVideoStreamTypeSyncVideo) {
    return VideoStreamType::kSyncVideo;
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

}  // namespace

std::unique_ptr<Streamer> MakeStreamer() {
  const std::string path =
      common::Config().GetString(defines::kVideoStreamSection, defines::kVideoStreamPathOption, {});
  if (path.empty()) {
    throw InvalidConfig{};
  }
  switch (StreamType()) {
    case VideoStreamType::kRtsp:
      return std::make_unique<StreamReader>(path);
    case VideoStreamType::kGstreamer:
      return std::make_unique<GstStreamer>(path, GetFrameSize());
    case VideoStreamType::kSyncVideo:
      return std::make_unique<SyncVideoStreamer>(GetFrameSize());
  }

  throw std::logic_error{"Incorrect Config parsing logic"};
}

}  // namespace streamer

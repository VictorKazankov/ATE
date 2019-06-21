#ifndef ATE_SERVER_STREAMER_FACTORY_H_
#define ATE_SERVER_STREAMER_FACTORY_H_

#include <memory>

#include "video_streaming/streamer.h"

namespace streamer {
std::unique_ptr<Streamer> MakeStreamer();
}

#endif  // ATE_SERVER_STREAMER_FACTORY_H_

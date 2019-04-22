#include "video_streaming/stream_reader.h"

streamer::StreamReader::StreamReader(const std::string& source) : capture_(source) {}

bool streamer::StreamReader::Frame(cv::Mat& frame) { return capture_.read(frame); }

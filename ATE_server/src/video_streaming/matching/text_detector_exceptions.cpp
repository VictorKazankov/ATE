#include "video_streaming/matching/text_detector_exceptions.h"

namespace detector {
TextDetectorInitializationError::TextDetectorInitializationError(const char* what_arg)
    : std::runtime_error{what_arg} {}

TextDetectorInitializationError::TextDetectorInitializationError(const std::string& what_arg)
    : std::runtime_error{what_arg} {}
}  // namespace detector

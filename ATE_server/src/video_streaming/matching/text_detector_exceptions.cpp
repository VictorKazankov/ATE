#include "video_streaming/matching/text_detector_exceptions.h"

namespace detector {
TextDetectorRuntimeError::TextDetectorRuntimeError(const char* what_arg) : std::runtime_error{what_arg} {}
TextDetectorRuntimeError::TextDetectorRuntimeError(const std::string& what_arg) : std::runtime_error{what_arg} {}

TextDetectorInitializationError::TextDetectorInitializationError(const char* what_arg)
    : TextDetectorRuntimeError{what_arg} {}
TextDetectorInitializationError::TextDetectorInitializationError(const std::string& what_arg)
    : TextDetectorRuntimeError{what_arg} {}

TextDetectorRecognitionError::TextDetectorRecognitionError(const char* what_arg) : TextDetectorRuntimeError{what_arg} {}
TextDetectorRecognitionError::TextDetectorRecognitionError(const std::string& what_arg)
    : TextDetectorRuntimeError{what_arg} {}
}  // namespace detector

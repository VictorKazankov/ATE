#ifndef ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_
#define ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_

#include <stdexcept>

namespace detector {

class TextDetectorRuntimeError : public std::runtime_error {
 public:
  explicit TextDetectorRuntimeError(const char* what_arg);
  explicit TextDetectorRuntimeError(const std::string& what_arg);
};

class TextDetectorInitializationError : public TextDetectorRuntimeError {
 public:
  explicit TextDetectorInitializationError(const char* what_arg);
  explicit TextDetectorInitializationError(const std::string& what_arg);
};

class TextDetectorRecognitionError : public TextDetectorRuntimeError {
 public:
  explicit TextDetectorRecognitionError(const char* what_arg);
  explicit TextDetectorRecognitionError(const std::string& what_arg);
};
}  // namespace detector

#endif  // ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_

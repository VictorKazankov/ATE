#ifndef ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_
#define ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_

#include <stdexcept>

namespace detector {

class TextDetectorInitializationError : public std::runtime_error {
 public:
  explicit TextDetectorInitializationError(const char* what_arg);
  explicit TextDetectorInitializationError(const std::string& what_arg);
};
}  // namespace detector

#endif  // ATE_SERVER_VIDEOSTREAMING_MATCHING_TEXT_DETECTOR_EXCEPTIONS_H_

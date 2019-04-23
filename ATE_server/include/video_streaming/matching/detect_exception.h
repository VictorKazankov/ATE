#ifndef ATE_SERVER_DETECT_EXCEPTIONS_H_
#define ATE_SERVER_DETECT_EXCEPTIONS_H_

#include <exception>

namespace detector {

class LookupError : public std::exception {
 public:
  const char* what() const noexcept { return "Pattern is not detected."; }
};

}  // namespace detector

#endif  // ATE_SERVER_DETECT_EXCEPTIONS_H_

#include "converters.h"

#include <memory>

namespace ATE {
namespace impl_detail {

// clang-format off

std::string ToStdString(System::String^ s) {
  using namespace System;
  using namespace System::Runtime::InteropServices;

  auto deleter = [](char* p) { Marshal::FreeHGlobal(IntPtr{static_cast<void*>(p)}); };

  std::unique_ptr<char, decltype(deleter)> chars{
      static_cast<char*>(Marshal::StringToHGlobalAnsi(s).ToPointer()), deleter};
  return std::string{chars.get()};
}

System::String^ ToSystemString(const std::string& s) {
  return gcnew System::String(s.c_str());
}

// clang-format on

}  // namespace impl_detail
}  // namespace ATE

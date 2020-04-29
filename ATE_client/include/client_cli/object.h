#ifndef ATE_CLIENT_CLIENT_CLI_OBJECT_H_
#define ATE_CLIENT_CLIENT_CLI_OBJECT_H_

#include "screen_rectangle.h"

namespace ATE {

// clang-format off

/// <summary>An object reference element</summary>
public ref class Object : public ScreenRectangle {
 public:
  /// <summary>Default construct Object.</summary>
  Object() : ScreenRectangle{0, 0, 0, 0} {}

  /// <summary>Object name.</summary>
  property System::String^ Name {
    System::String^ get() { return name_; }
    void set(System::String^ value) { name_ = value; }
  }

 private:
  System::String^ name_;
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_OBJECT_H_

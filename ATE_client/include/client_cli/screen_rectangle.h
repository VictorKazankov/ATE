#ifndef ATE_CLIENT_CLIENT_CLI_SCREEN_RECTANGLE_H_
#define ATE_CLIENT_CLIENT_CLI_SCREEN_RECTANGLE_H_

#include "screen_point.h"

namespace ATE {

// clang-format off

/// <summary>A rectangle element with screen position and size properties.</summary>
public ref class ScreenRectangle {
 public:
  /** <summary>
        Construct ScreenRectangle with the top left point (<paramref name="x"/>, <paramref name="y"/>),
        and of size <paramref name="width"/>x<paramref name="height"/>.
      </summary>
  **/
  ScreenRectangle(int x, int y, int width, int height) : x_{x}, y_{y}, w_{width}, h_{height} {}

  /// <summary>X coordinate of top left point.</summary>
  property int X {
    int get() { return x_; }
    void set(int value) { x_ = value; }
  }

  /// <summary>Y coordinate of top left point.</summary>
  property int Y {
    int get() { return y_; }
    void set(int value) { y_ = value; }
  }

  /// <summary>Rectangle width.</summary>
  property int Width {
    int get() { return w_; }
    void set(int value) { w_ = value; }
  }

  /// <summary>Rectangle height.</summary>
  property int Height {
    int get() { return h_; }
    void set(int value) { h_ = value; }
  }

  /// <summary>Returns the center of the rectangle object as a ScreenPoint object.</summary>
  ScreenPoint Center() { return ScreenPoint{w_ / 2 + x_, h_ / 2 + y_}; }

 private:
  int x_, y_, w_, h_;
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_SCREEN_RECTANGLE_H_

#ifndef ATE_CLIENT_CLIENT_CLI_SCREEN_POINT_H_
#define ATE_CLIENT_CLIENT_CLI_SCREEN_POINT_H_

namespace ATE {

// clang-format off

/// <summary>A point element representing a position on screen.</summary>
public value class ScreenPoint {
 public:
  /** <summary>
        Construct ScreenPoint with the coordinates <paramref name="x"/> and <paramref name="y"/>
      </summary>
  **/
  ScreenPoint(int x, int y) : x_{x}, y_{y} {}

  /// <summary>X coordinate.</summary>
  property int X {
    int get() { return x_; }
    void set(int value) { x_ = value; }
  }

  /// <summary>Y coordinate.</summary>
  property int Y {
    int get() { return y_; }
    void set(int value) { y_ = value; }
  }

 private:
  int x_, y_;
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_SCREEN_POINT_H_

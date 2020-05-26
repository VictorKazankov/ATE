#ifndef ATE_CLIENT_CLIENT_CLI_OBJECT_H_
#define ATE_CLIENT_CLIENT_CLI_OBJECT_H_

#include "enums.h"
#include "errors.h"
#include "screen_point.h"
#include "screen_rectangle.h"

namespace ATE {

// clang-format off

/// <summary>An object reference element</summary>
public ref class Object : public ScreenRectangle {
 public:
  /// <summary>Default construct Object.</summary>
  Object() : ScreenRectangle{0, 0, 0, 0} {}

  /** <summary>
        Method returns a true value if the object with the symbolic or real (multi-property)
        <paramref name="objectName"/> exists; otherwise it returns a false value.
      </summary>
      <param>
        <param name="objectName">Symbolic or real (multi-property) Object name to check existence of</param>
      </param>
      <returns>
        true if the object exists, otherwise false."
      </returns>
      <exception cref="ATE::NoConnectionEstablished">Thrown if no connection to ATE server</exception>
      <exception cref="ATE::VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
  **/
  bool Exists(System::String^ objectName);

  /** <summary>
        Method returns a true value if the object with the symbolic or real (multi-property)
        <paramref name="objectName"/> exists; otherwise it returns a false value.
      </summary>
      <param>
        <param name="objectName">Symbolic or real (multi-property) Object name to check existence of</param>
        <param name="topLeft">Top-left coordinate of capture area.</param>
        <param name="bottomRight">Bottom-right coordinate of capture area.</param>
      </param>
      <returns>
        true if the object exists, otherwise false."
      </returns>
      <exception cref="ATE::NoConnectionEstablished">Thrown if no connection to ATE server</exception>
      <exception cref="ATE::VideoStreamingError">Thrown if video stream is not available</exception>
      <exception cref="System::ArgumentException">Thrown if one or more of the arguments provided are not valid</exception>
      <exception cref="System::Exception">Thrown if internal error has occured.</exception>
      <exception cref="InvalidRectangleCoordinates">Thrown if top-left and bottom-right coordinates are mixed up or
                                                    rectangle has zero height/width or is out of frame boundaries.</exception>
  **/
  bool Exists(System::String^ objectName, ScreenPoint topLeft, ScreenPoint bottomRight);

  /// <summary>Top-left point of bounding rect of the Object.</summary>
  property ScreenPoint TopLeft {
    ScreenPoint get() { return ScreenPoint{x_top_left_, y_top_left_}; }
  }

  /// <summary>Bottom-right point of bounding rect of the Object.</summary>
  property ScreenPoint BottomRight {
    ScreenPoint get() { return ScreenPoint{x_bottom_right_, y_bottom_right_}; }
  }

  /// <summary>X-coordinate of top-left point of bounding rect of the Object.</summary>
  property int TopLeftX {
    int get() { return x_top_left_; }
    void set(int value) { x_top_left_ = value; }
  }

  /// <summary>Y-coordinate of top-left point of bounding rect of the Object.</summary>
  property int TopLeftY {
    int get() { return y_top_left_; }
    void set(int value) { y_top_left_ = value; }
  }

  /// <summary>X-coordinate of bottom-right point of bounding rect of the Object.</summary>
  property int BottomRightX {
    int get() { return x_bottom_right_; }
    void set(int value) { x_bottom_right_ = value; }
  }

  /// <summary>Y-coordinate of bottom-right point of bounding rect of the Object.</summary>
  property int BottomRightY {
    int get() { return y_bottom_right_; }
    void set(int value) { y_bottom_right_ = value; }
  }

  /// <summary>Parent screen resolution width.</summary>
  property int ParentWidth {
    int get() { return parent_width_; }
    void set(int value) { parent_width_ = value; }
  }

  /// <summary>Parent screen resolution height.</summary>
  property int ParentHeight {
    int get() { return parent_height_; }
    void set(int value) { parent_height_ = value; }
  }

  /// <summary>SYNC product name of the Object.</summary>
  property System::String^ SyncVersion {
    System::String^ get() { return sync_version_; }
    void set(System::String^ value) { sync_version_ = value; }
  }

  /// <summary>SYNC build revision of the Object.</summary>
  property System::String^ SyncBuildVersion {
    System::String^ get() { return build_version_; }
    void set(System::String^ value) { build_version_ = value; }
  }

  /// <summary>HMI light mode of the Object.</summary>
  property CollectionMode SyncCollectionMode {
    CollectionMode get() { return mode_; }
    void set(CollectionMode value) { mode_ = value; }
  }

  /// <summary>Object name.</summary>
  property System::String^ Name {
    System::String^ get() { return name_; }
    void set(System::String^ value) { name_ = value; }
  }

  /// <summary>Parent screen name of the Object.</summary>
  property System::String^ ParentScreen {
    System::String^ get() { return parent_screen_; }
    void set(System::String^ value) { parent_screen_ = value; }
  }

 private:
  int x_top_left_{-1};
  int y_top_left_{-1};
  int x_bottom_right_{-1};
  int y_bottom_right_{-1};
  int parent_width_{-1};
  int parent_height_{-1};
  System::String^ sync_version_{gcnew System::String("")};
  System::String^ build_version_{gcnew System::String("")};
  CollectionMode mode_{CollectionMode::None};
  System::String^ name_{gcnew System::String("")};
  System::String^ parent_screen_{gcnew System::String("")};
};

// clang-format on

}  // namespace ATE

#endif  // ATE_CLIENT_CLIENT_CLI_OBJECT_H_

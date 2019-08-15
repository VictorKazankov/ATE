#ifndef SQUISH_SQUISH_TYPES_H_
#define SQUISH_SQUISH_TYPES_H_

#include <string>
#include "utils/geometry_types.h"

namespace squish {

struct Object : common::Rect {
  Object(){};
  Object(int x, int y) : common::Rect{x, y, 0, 0} {};
  Object(int x, int y, int width, int height) : common::Rect{x, y, width, height} {};

  /**
   * @brief This function verifies the object with the symbolic
   * or real (multi-property) name objectName exists on the screen.
   * @param object_name - name of the object for verifying existence on the screen
   * @return returns a true value if the object with the symbolic
   * or real (multi-property) name objectName exists otherwise false
   **/
  bool Exists(const std::string& object_name) const;

  std::string name;
  std::string type;
};

}  // namespace squish

#endif  // SQUISH_SQUISH_TYPES_H_

#include "squish/squish_types.h"
#include "squish/squish_API.h"

using namespace squish;

bool Object::Exists(const std::string& object_name) const {
  try {
    API::WaitForObject(object_name);
    return true;
  } catch (const LookupError&) {
    return false;
  }
}

#include "squish/squish_types.h"
#include "squish/squish_API.h"

using namespace squish;

bool Object::Exists(const std::string& /*object_name*/) const {
  try {
    //API::WaitForObject(object_name); //TODO
    return true;
  } catch (const std::runtime_error&) {
    return false;
  }
}

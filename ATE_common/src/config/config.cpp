#include "config/config.h"

#include <limits>

#include "exceptions.h"

namespace config {

using std::string;

Reader::Reader(const string& from_file) : reader_(from_file) {
  if (reader_.ParseError()) throw FileReadFailure(from_file);
}

string Reader::GetString(const string& section, const string& option, const string& default_value) const {
  return reader_.Get(section, option, default_value);
}

int Reader::GetInt(const string& section, const string& option, const int default_value) const {
  const long result = reader_.GetInteger(section, option, default_value);

  constexpr int kIntMin = std::numeric_limits<int>::min();
  constexpr int kIntMax = std::numeric_limits<int>::max();
  if (result <= kIntMin) {
    return kIntMin;
  } else if (result >= kIntMax) {
    return kIntMax;
  } else {
    return static_cast<int>(result);
  }
}

double Reader::GetDouble(const string& section, const string& option, const double default_value) const {
  return reader_.GetReal(section, option, default_value);
}

bool Reader::GetBool(const string& section, const string& option, const bool default_value) const {
  return reader_.GetBoolean(section, option, default_value);
}

}  // namespace config

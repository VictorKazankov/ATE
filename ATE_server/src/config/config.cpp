#include "config/config.h"

#include <limits>

#include "config/config_exceptions.h"

namespace config {

using std::string;

Reader::Reader(const string& from_file) : reader_(from_file) {
  if (reader_.ParseError()) throw ConfigFileReadFailure();
}

string Reader::GetString(const string& section, const string& option, const string& default_value) const {
  return reader_.Get(section, option, default_value);
}

int Reader::GetInt(const string& section, const string& option, const int default_value) const {
  const long result = reader_.GetInteger(section, option, default_value);
  return std::numeric_limits<int>::min() <= result && result <= std::numeric_limits<int>::max()
             ? static_cast<int>(result)
             : default_value;
}

double Reader::GetDouble(const string& section, const string& option, const double default_value) const {
  return reader_.GetReal(section, option, default_value);
}

bool Reader::GetBool(const string& section, const string& option, const bool default_value) const {
  return reader_.GetBoolean(section, option, default_value);
}

}  // namespace config

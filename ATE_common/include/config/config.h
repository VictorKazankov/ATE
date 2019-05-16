#ifndef ATE_COMMON_CONFIG_READER_H_
#define ATE_COMMON_CONFIG_READER_H_

#include <string>

#include <INIReader.h>

namespace config {

/**
 * @class Reader
 *
 * @brief Class to read config values from *.ini file.
 *
 * This class is a wrapper for an INIReader of an INIH lib. It provides functions to read standard INI types.
 * The default value must be provided which is returned in case of read error.
 *
 * Ini file name is needed in order to instantiate a class. In case of file open or parse error
 * the config::ConfigFileReadFailure exception is thrown.
 */
class Reader {
 public:
  /**
   * @brief Create a Reader from a given *.ini file name
   */
  explicit Reader(const std::string& from_file);

  /**
   * @brief Get a value of a String type
   * @param section an INI section name which contains desired option name
   * @param option an INI option name inside a section which contains desired value
   * @param default_value a default value which will be returned in case of any read errors
   * @return value read from INI file or default_value in case of error
   *
   * This functions matches a section and option name to find a value in a INI file, and returns
   * the value as string type.
   *
   * Empty string is an accepted value.
   */
  std::string GetString(const std::string& section, const std::string& option, const std::string& default_value) const;

  /**
   * @brief Get a value of an Integer type
   * @param section an INI section name which contains desired option name
   * @param option an INI option name inside a section which contains desired value
   * @param default_value a default value which will be returned in case of any read errors
   * @return value read from INI file or default_value in case of error
   *
   * This functions matches a section and option name to find a value in a INI file, and converts
   * the value to an int type.
   *
   * Value must be in the [INT_MIN, INT_MAX] range.
   */
  int GetInt(const std::string& section, const std::string& option, const int default_value) const;

  /**
   * @brief Get a value of a Real type
   * @param section an INI section name which contains desired option name
   * @param option an INI option name inside a section which contains desired value
   * @param default_value a default value which will be returned in case of any read errors
   * @return value read from INI file or default_value in case of error
   *
   * This functions matches a section and option name to find a value in a INI file, and converts
   * the value to a double type.
   */
  double GetDouble(const std::string& section, const std::string& option, const double default_value) const;

  /**
   * @brief Get a value of a Boolean type
   * @param section an INI section name which contains desired option name
   * @param option an INI option name inside a section which contains desired value
   * @param default_value a default value which will be returned in case of any read errors
   * @return vlaue read from INI file or default_value in case of error
   *
   * This functions matches a section and option name to find a value in a INI file, and converts
   * the value to a bool type.
   *
   * Pairs of values treated as a bool:
   * - false/true
   * - 0/1
   * - no/yes
   * - off/on
   */
  bool GetBool(const std::string& section, const std::string& option, const bool default_value) const;

 private:
  INIReader reader_;
};

}  // namespace config

#endif  // ATE_COMMON_CONFIG_READER_H_

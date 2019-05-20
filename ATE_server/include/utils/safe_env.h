#ifndef ATE_COMMON_UTILS_SAFE_ENV_H_
#define ATE_COMMON_UTILS_SAFE_ENV_H_

#include <string>
#include <utility>

namespace safe_env {

/**
 * @brief Get an environment variable
 *
 * @param name - enviroment variable name
 *
 * @return std::pair<std::string, bool>:
 * first - environment variable's value,
 * or an empty string if variable isn't set
 * second - identify if a variable is set
 *
 * @throws Errors from the underlying operating system that would prevent lock from meeting its specifications.
 */
std::pair<std::string, bool> Get(const char* name);

/**
 * @brief Adds the 'name' to the environment with the 'val' value, if name doesn't already exist.
 * If 'name' does exist in the environment, then its value is changed to 'val' if overwrite is true.
 * If overwrite is false, then the value of 'name' isn't changed.
 *
 * @param name - environment variable name
 * @param val - (new) value for 'name'
 * @param overwrite - overwrite flag
 *
 * @throws system_error with next error conditions:
 * std::errc::invalid_argument if
 * 'name' is null, points to a string of length 0, or contains an '=' character or 'value' is null
 * std::errc::not_enough_memory if insufficient memory to add a new variable to the environment. or errors from the
 * underlying operating system that would prevent lock from meeting its specifications.
 */
void Set(const char* name, const char* value, bool overwrite);

/**
 * @brief Deletes the 'name' from the environment.
 * If 'name' doesn't exist in the environment, then the function succeeds, and the environment is unchanged.
 *
 * @param name - environment variable name
 *
 * @throws system_error with next error conditions:
 * std::errc::invalid_argument if name is null, points to a string of length 0, or contains an '=' character.
 * or errors from the underlying operating system that would prevent lock from meeting its specifications.
 */
void Unset(const char* name);

}  // namespace safe_env

#endif  // ATE_COMMON_UTILS_SAFE_ENV_H_

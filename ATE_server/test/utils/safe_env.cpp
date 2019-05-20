#include "utils/safe_env.h"

#include <string>
#include <system_error>
#include <utility>

#include <gtest/gtest.h>

namespace {

constexpr auto kDefinedMessage = "Test environment variable must be defined";
constexpr auto kUndefinedMessage = "An environment variable with a tested name must not be defined";
constexpr auto kVarName = "SafeEnvTestVariable";

TEST(SafeEnvTest, SingleVariableTest) {
  constexpr auto var_val1 = "it's 1";
  constexpr auto var_val2 = "I'm second value";

  // Write variable
  ASSERT_NO_THROW(safe_env::Set(kVarName, var_val1, false));

  std::pair<std::string, bool> get_result = safe_env::Get(kVarName);
  EXPECT_TRUE(get_result.second) << kDefinedMessage;
  EXPECT_EQ(get_result.first, var_val1);

  // Overwrite without force
  EXPECT_NO_THROW(safe_env::Set(kVarName, var_val2, false));

  get_result = safe_env::Get(kVarName);
  EXPECT_TRUE(get_result.second) << kDefinedMessage;
  EXPECT_EQ(get_result.first, var_val1);

  // Overwrite with force
  EXPECT_NO_THROW(safe_env::Set(kVarName, var_val2, true));

  get_result = safe_env::Get(kVarName);
  EXPECT_TRUE(get_result.second) << kDefinedMessage;
  EXPECT_EQ(get_result.first, var_val2);

  // Unset
  EXPECT_NO_THROW(safe_env::Unset(kVarName));

  get_result = safe_env::Get(kVarName);
  EXPECT_FALSE(get_result.second) << kUndefinedMessage;
  EXPECT_TRUE(get_result.first.empty());
}

TEST(SafeEnvTest, NonExistingVariable) {
  constexpr auto non_var_name = "There is no environment variable";

  EXPECT_NO_THROW(safe_env::Unset(non_var_name));

  const auto get_result = safe_env::Get(non_var_name);
  EXPECT_FALSE(get_result.second) << kUndefinedMessage;
  EXPECT_TRUE(get_result.first.empty());
}

TEST(SafeEnvTest, InvalidNames) {
  const char* const invalid_var_names[] = {nullptr, "", "name=name"};
  for (const auto invalid_name : invalid_var_names) {
    try {
      safe_env::Set(invalid_name, "Value", false);
    } catch (const std::system_error& e) {
      EXPECT_EQ(e.code(), std::errc::invalid_argument);
    }

    try {
      safe_env::Set(invalid_name, "ValueForce", true);
    } catch (const std::system_error& e) {
      EXPECT_EQ(e.code(), std::errc::invalid_argument);
    }

    try {
      safe_env::Unset(invalid_name);
    } catch (const std::system_error& e) {
      EXPECT_EQ(e.code(), std::errc::invalid_argument);
    }

    const auto get_result = safe_env::Get(invalid_name);
    EXPECT_FALSE(get_result.second) << kUndefinedMessage;
    EXPECT_TRUE(get_result.first.empty());
  }
}

TEST(SafeEnvTest, EmptyValue) {
  ASSERT_NO_THROW(safe_env::Set(kVarName, "", true));

  auto get_result = safe_env::Get(kVarName);
  EXPECT_TRUE(get_result.second) << kDefinedMessage;
  EXPECT_TRUE(get_result.first.empty()) << "An environment variable must be empty";

  EXPECT_NO_THROW(safe_env::Unset(kVarName));
}

TEST(SafeEnvTest, NullValue) {
  try {
    safe_env::Set(kVarName, nullptr, true);
  } catch (const std::system_error& e) {
    EXPECT_EQ(e.code(), std::errc::invalid_argument);
  }

  auto get_result = safe_env::Get(kVarName);
  EXPECT_FALSE(get_result.second) << kUndefinedMessage;
  EXPECT_TRUE(get_result.first.empty());

  EXPECT_NO_THROW(safe_env::Unset(kVarName));
}

}  // namespace

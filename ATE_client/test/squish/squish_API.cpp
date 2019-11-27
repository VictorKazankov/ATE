#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define private public
#define protected public
#include "squish/squish_API.h"

using ::testing::_;  // Matcher for parameters
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {

using namespace squish;

class APITestPredefinedEnvVar : public ::testing::Test {
 public:
  void SetUp() override;
  void TearDown() override;

 private:
  static const char* kConfigEnvVar;
  char* config_env_var_old_{nullptr};
};

const char* APITestPredefinedEnvVar::kConfigEnvVar{"ATE_CLIENT_CONFIG"};

void APITestPredefinedEnvVar::SetUp() {
  const auto kInvalidConfigValue = "";
  config_env_var_old_ = std::getenv(kConfigEnvVar);
  ASSERT_EQ(0, setenv(kConfigEnvVar, kInvalidConfigValue, 1)) << "Couldn't set environment variable " << kConfigEnvVar;
}

void APITestPredefinedEnvVar::TearDown() {
  ASSERT_EQ(0, setenv(kConfigEnvVar, config_env_var_old_ ? config_env_var_old_ : "", 1))
      << "Couldn't restore value of the environment variable " << kConfigEnvVar;
}

TEST_F(APITestPredefinedEnvVar, AttachToApplication_SetInvalidConfig_Exception) {
  EXPECT_THROW(API::AttachToApplication(""), std::runtime_error);
}

TEST(APITest, WaitForObject_NotConnected_Exception) {
  const int kTimeMs = 100;

  EXPECT_THROW(API::WaitForObject(squish::Object{}), std::runtime_error);
  EXPECT_THROW(API::WaitForObject("test_unknown_name"), std::runtime_error);
  EXPECT_THROW(API::WaitForObject(squish::Object{}, kTimeMs), std::runtime_error);
  EXPECT_THROW(API::WaitForObject("test_unknown_name", kTimeMs), std::runtime_error);
}

TEST(APITest, TapObject_NotConnected_Exception) {
  EXPECT_THROW(API::TapObject(common::Point{}), std::runtime_error);
  EXPECT_THROW(API::TapObject(common::Rect{}), std::runtime_error);
  EXPECT_THROW(API::TapObject(Object{}), std::runtime_error);
}

TEST(APITest, TouchAndDrag_NotConnected_Exception) {
  EXPECT_THROW(API::TouchAndDrag(Object{}, 1, 2, 3, 4, common::squish::ModifierState::NONE), std::runtime_error);
  EXPECT_THROW(API::TouchAndDrag("test_unknown_name", 1, 2, 3, 4, common::squish::ModifierState::NONE),
               std::runtime_error);
}

TEST(APITest, ChangeSyncIconDB_NotConnected_Exception) {
  EXPECT_THROW(API::ChangeSyncIconDB("test_unknown_sync", "test_unknown_build"), std::runtime_error);
}

TEST(APITest, ChangeSyncMode_NotConnected_Exception) {
  EXPECT_THROW(API::ChangeSyncMode(common::squish::CollectionMode::DAY), std::runtime_error);
}

}  // namespace

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "squish/squish_API.h"

#define private public
#define protected public
#include "squish/application_context.h"

using ::testing::_;  // Matcher for parameters
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {

using namespace squish;

namespace {
constexpr int kId = 1;
const int kTimeMs = 100;
const std::string kObjName = "test_name";
const squish::Wildcard kWidcard{"test_name"};
const std::string wfo_response(R"({"id":6,"jsonrpc":"2.0","result":{"height":15,"width":45,"x":44,"y":419}})");
const std::string tap_response(R"({"id":10,"jsonrpc":"2.0","result":true})");
}  // namespace

class MockATEInteraction : public interaction::Interaction {
 public:
  MOCK_CONST_METHOD0(host, std::string());
  MOCK_CONST_METHOD0(port, std::string());
  MOCK_METHOD0(Connect, void());
  MOCK_CONST_METHOD0(IsConnectionOpened, bool());
  MOCK_METHOD1(SendCommand, std::string(const std::string&));
};

class SquishApiTest : public ::testing::Test {
 public:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<squish::ApplicationContext> application_context_;
  std::shared_ptr<MockATEInteraction> mock_{nullptr};
  API::SquishApi api_;
};

void SquishApiTest::SetUp() {
  application_context_ = std::make_unique<squish::ApplicationContext>();
  auto mock = std::make_unique<MockATEInteraction>();
  application_context_->Attach(std::move(mock));
  mock_ = std::make_shared<MockATEInteraction>();
  ASSERT_TRUE(mock_);
}

void SquishApiTest::TearDown() {
  application_context_.reset();
  mock_ = nullptr;
}

TEST_F(SquishApiTest, WaitForObject_StringName_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kObjName);
}

TEST_F(SquishApiTest, WaitForObject_StringNameAndTime_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kObjName, kTimeMs);
}

TEST_F(SquishApiTest, WaitForObject_Object_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, squish::Object{});
}

TEST_F(SquishApiTest, WaitForObject_ObjectAndTime_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, squish::Object{}, kTimeMs);
}

TEST_F(SquishApiTest, WaitForObject_Wildcard_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kWidcard);
}

TEST_F(SquishApiTest, WaitForObject_WildcardAndTime_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kWidcard, kTimeMs);
}

TEST_F(SquishApiTest, TapObject_Point_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(tap_response));

  api_.TapObject(mock_, kId, common::Point{});
}

TEST_F(SquishApiTest, TapObject_Rect_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(tap_response));

  api_.TapObject(mock_, kId, common::Rect{});
}

TEST_F(SquishApiTest, TapObject_Object_CallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(tap_response));

  api_.TapObject(mock_, kId, Object{});
}

/* TODO
TEST_F(SquishApiTest, AttachToApplication_SetEmptyAppName_CallOnce) {
  API::SquishApi api;

  EXPECT_CALL(*mock_, SendCommand(_)).Times(1);

  api.AttachToApplication(mock_, "");
}
*/

/*
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

TEST(APITest, TouchAndDrag_NotConnected_Exception) {
  EXPECT_THROW(API::TouchAndDrag(Object{}, 1, 2, 3, 4, common::squish::ModifierState::NONE), std::runtime_error);
  EXPECT_THROW(API::TouchAndDrag("test_unknown_name", 1, 2, 3, 4, common::squish::ModifierState::NONE),
               std::runtime_error);
}

TEST(APITest, PressAndHold_NotConnected_Exception) {
  EXPECT_THROW(API::PressAndHold(common::Point{}), std::runtime_error);
  EXPECT_THROW(API::PressAndHold(common::Rect{}), std::runtime_error);
  EXPECT_THROW(API::PressAndHold(Object{}), std::runtime_error);
}

TEST(APITest, PressRelease_NotConnected_Exception) {
  EXPECT_THROW(API::PressRelease(common::Point{}), std::runtime_error);
  EXPECT_THROW(API::PressRelease(common::Rect{}), std::runtime_error);
  EXPECT_THROW(API::PressRelease(Object{}), std::runtime_error);
}

TEST(APITest, ChangeSyncIconDB_NotConnected_Exception) {
  EXPECT_THROW(API::ChangeSyncIconDB("test_unknown_sync", "test_unknown_build"), std::runtime_error);
}

TEST(APITest, ChangeSyncMode_NotConnected_Exception) {
  EXPECT_THROW(API::ChangeSyncMode(common::squish::CollectionMode::DAY), std::runtime_error);
}

TEST(APITest, GetText_NotConnected_Exception) { EXPECT_THROW(API::GetText(0, 1, 2, 3), std::runtime_error); }

TEST(APITest, GetScreenshot_NotConnected_Exception) {
  EXPECT_THROW(API::GetScreenshot("filename", "location"), std::runtime_error);
}

*/
}  // namespace

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Helper macros for testing private section
#define protected public
#define private public
#include "squish/squish_API.h"

using ::testing::_;
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
const std::string general_response(R"({"id":10,"jsonrpc":"2.0","result":true})");
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
 protected:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<squish::ApplicationContext> application_context_;
  std::shared_ptr<MockATEInteraction> mock_{nullptr};
  API::SquishApi api_;
  squish::Object screen_rectangle_{0, 0, 0, 0};
  uint64_t correlation_id_ = 0;
  int timeout_msec_ = 0;
};

void SquishApiTest::SetUp() {
  application_context_ = std::make_unique<squish::ApplicationContext>();
  auto mock = std::make_shared<MockATEInteraction>();
  application_context_->Attach(mock);
  mock_ = std::make_shared<MockATEInteraction>();
  ASSERT_TRUE(mock_);
}

void SquishApiTest::TearDown() {
  application_context_.reset();
  mock_.reset();
}

TEST_F(SquishApiTest, AttachToApplication_SetAteInteractionMock_HostAndPortEqualForInteractionAndAppContext) {
  EXPECT_CALL(*mock_, SendCommand(_)).Times(0);
  EXPECT_CALL(*mock_, host()).WillRepeatedly(Return("127.0.0.1"));
  EXPECT_CALL(*mock_, port()).WillRepeatedly(Return("8800"));

  const squish::ApplicationContext& context = api_.AttachToApplication(mock_);

  EXPECT_EQ(context.host(), mock_->host());
  EXPECT_EQ(context.port(), mock_->port());
}

TEST_F(SquishApiTest, AttachToApplication_SetAnotherAteInteractionAsFirstIsRunning_AppContextNotChanged) {
  EXPECT_CALL(*mock_, SendCommand(_)).Times(0);
  EXPECT_CALL(*mock_, host()).WillRepeatedly(Return("127.0.0.1"));
  EXPECT_CALL(*mock_, port()).WillRepeatedly(Return("8800"));
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(true));

  std::shared_ptr<MockATEInteraction> another_mock = std::make_shared<MockATEInteraction>();
  EXPECT_CALL(*another_mock, SendCommand(_)).Times(0);
  EXPECT_CALL(*another_mock, host()).WillRepeatedly(Return("localhost"));
  EXPECT_CALL(*another_mock, port()).WillRepeatedly(Return("5000"));
  EXPECT_CALL(*another_mock, IsConnectionOpened()).Times(0);

  api_.AttachToApplication(mock_);
  const squish::ApplicationContext& context = api_.AttachToApplication(another_mock);

  EXPECT_EQ(context.host(), mock_->host());
  EXPECT_EQ(context.port(), mock_->port());
}

TEST_F(SquishApiTest, AttachToApplication_SetAnotherAteInteractionAsFirstIsNotRunning_AppContextChanged) {
  EXPECT_CALL(*mock_, SendCommand(_)).Times(0);
  EXPECT_CALL(*mock_, host()).WillRepeatedly(Return("127.0.0.1"));
  EXPECT_CALL(*mock_, port()).WillRepeatedly(Return("8800"));
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(false));

  std::shared_ptr<MockATEInteraction> another_mock = std::make_shared<MockATEInteraction>();
  EXPECT_CALL(*another_mock, SendCommand(_)).Times(0);
  EXPECT_CALL(*another_mock, host()).WillRepeatedly(Return("localhost"));
  EXPECT_CALL(*another_mock, port()).WillRepeatedly(Return("5000"));
  EXPECT_CALL(*another_mock, IsConnectionOpened()).Times(0);

  api_.AttachToApplication(mock_);
  const squish::ApplicationContext& context = api_.AttachToApplication(another_mock);

  EXPECT_EQ(context.host(), another_mock->host());
  EXPECT_EQ(context.port(), another_mock->port());
}

TEST_F(SquishApiTest, WaitForObject_StringName_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kObjName);
}

TEST_F(SquishApiTest, WaitForObject_StringNameAndTime_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kObjName, kTimeMs);
}

TEST_F(SquishApiTest, WaitForObject_Object_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, squish::Object{});
}

TEST_F(SquishApiTest, WaitForObject_ObjectAndTime_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, squish::Object{}, kTimeMs);
}

TEST_F(SquishApiTest, WaitForObject_Wildcard_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kWidcard);
}

TEST_F(SquishApiTest, WaitForObject_WildcardAndTime_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(wfo_response));

  api_.WaitForObject(mock_, kId, kWidcard, kTimeMs);
}

TEST_F(SquishApiTest, TapObject_Point_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.TapObject(mock_, kId, common::Point{});
}

TEST_F(SquishApiTest, TapObject_Rect_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.TapObject(mock_, kId, common::Rect{});
}

TEST_F(SquishApiTest, TapObject_Object_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.TapObject(mock_, kId, Object{});
}

TEST_F(SquishApiTest, TouchAndDrag_Object_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.TouchAndDrag(mock_, kId, Object{}, 1, 2, 3, 4, common::squish::ModifierState::kNone);
}

TEST_F(SquishApiTest, TouchAndDrag_StringName_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.TouchAndDrag(mock_, kId, "test_unknown_name", 1, 2, 3, 4, common::squish::ModifierState::kNone);
}

TEST_F(SquishApiTest, PressAndHold_Point_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressAndHold(mock_, kId, common::Point{});
}

TEST_F(SquishApiTest, PressAndHold_Rect_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressAndHold(mock_, kId, common::Rect{});
}

TEST_F(SquishApiTest, PressAndHold_Object_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressAndHold(mock_, kId, Object{});
}

TEST_F(SquishApiTest, PressRelease_Point_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressRelease(mock_, kId, common::Point{});
}

TEST_F(SquishApiTest, PressRelease_Rect_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressRelease(mock_, kId, common::Rect{});
}

TEST_F(SquishApiTest, PressRelease_Object_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.PressRelease(mock_, kId, squish::Object{});
}

TEST_F(SquishApiTest, SetDefaultWaitForObjectTimeout_TimeoutMsec_Success) {
  timeout_msec_ = 1;
  api_.SetDefaultWaitForObjectTimeout(timeout_msec_);
  EXPECT_EQ(timeout_msec_, api_.default_wait_for_object_timeout_in_ms_);
}

TEST_F(SquishApiTest, LongPress_InvalidParams_MessageNotSent) {
  EXPECT_CALL(*mock_, SendCommand(_)).Times(0);

  int x1{1}, x2{0}, y1{0}, y2{1}, x3{-1}, y3{-1};

  api_.LongPress(mock_, correlation_id_, screen_rectangle_, x1, y1, timeout_msec_);
  api_.LongPress(mock_, correlation_id_, screen_rectangle_, x2, y2, timeout_msec_);
  api_.LongPress(mock_, correlation_id_, screen_rectangle_, x3, y1, timeout_msec_);
  api_.LongPress(mock_, correlation_id_, screen_rectangle_, x2, y3, timeout_msec_);
}

TEST_F(SquishApiTest, LongPress_ValidParams_SendCommandCallOnce) {
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(general_response));

  api_.LongPress(mock_, correlation_id_, screen_rectangle_, timeout_msec_);
}

TEST_F(SquishApiTest, Exists_ValidResponce_ExpectTrue) {
  std::string response =
      R"({\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"WaitForObject\",
          \"params\":{\"name\":\"test_name\",\"timeout_msec_\":1}})";
  EXPECT_CALL(*mock_, SendCommand(_)).WillOnce(Return(response));

  EXPECT_TRUE(api_.Exists(mock_, correlation_id_, kObjName));
}

}  // namespace

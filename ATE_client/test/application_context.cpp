#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define private public
#define protected public
#include "squish/application_context.h"

using ::testing::_;  // Matcher for parameters
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {

class MockATEInteraction : public interaction::Interaction {
 public:
  MOCK_CONST_METHOD0(host, std::string());
  MOCK_CONST_METHOD0(port, std::string());
  MOCK_METHOD0(Connect, void());
  MOCK_CONST_METHOD0(IsConnectionOpened, bool());
  MOCK_METHOD2(SendCommand, squish::Object(interaction::Method, const std::string&));
};

class ApplicationContextTest : public ::testing::Test {
 public:
  void SetUp() override;
  void TearDown() override;

  std::unique_ptr<squish::ApplicationContext> application_context_;
  MockATEInteraction* mock_{nullptr};
};

void ApplicationContextTest::SetUp() {
  application_context_ = std::make_unique<squish::ApplicationContext>();
  auto mock = std::make_unique<MockATEInteraction>();
  application_context_->Attach(std::move(mock));
  mock_ = static_cast<MockATEInteraction*>(application_context_->ate_interaction.get());
  ASSERT_TRUE(mock_);
}

void ApplicationContextTest::TearDown() {
  application_context_.reset();
  mock_ = nullptr;
}

TEST_F(ApplicationContextTest, IsRunning_IsConnectionOpened_True) {
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(true));
  EXPECT_TRUE(application_context_->IsRunning());
}

// TODO TEST_F(ApplicationContextTest, Detach_*)

TEST_F(ApplicationContextTest, SendCommand_IsRunningTrue_Success) {
  const auto kMethod = interaction::Method::kWaitForObject;
  const std::string kCommand = "command";

  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(true));
  EXPECT_CALL(*mock_, SendCommand(kMethod, kCommand)).WillOnce(Return(squish::Object{10, 20, 100, 200}));

  auto object = application_context_->SendCommand(kMethod, kCommand);

  EXPECT_EQ(object.x, 10);
  EXPECT_EQ(object.y, 20);
  EXPECT_EQ(object.width, 100);
  EXPECT_EQ(object.height, 200);
}

TEST_F(ApplicationContextTest, SendCommand_IsRunningFalse_Failure) {
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(false));
  EXPECT_CALL(*mock_, SendCommand(_, _)).Times(0);

  const auto kMethod = interaction::Method::kWaitForObject;
  const std::string kCommand = "command";

  EXPECT_THROW(application_context_->SendCommand(kMethod, kCommand), std::runtime_error);
}

TEST_F(ApplicationContextTest, Host_Predefined_Equal) {
  const std::string kHost = "localhost";
  EXPECT_CALL(*mock_, host()).WillOnce(Return(kHost));
  EXPECT_STREQ(application_context_->host().c_str(), kHost.c_str());
}

TEST_F(ApplicationContextTest, Port_Predefined_Equal) {
  const std::string kPort = "2121";
  EXPECT_CALL(*mock_, port()).WillOnce(Return(kPort));
  EXPECT_STREQ(application_context_->port().c_str(), kPort.c_str());
}
}  // namespace

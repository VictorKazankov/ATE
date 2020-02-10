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
  MOCK_METHOD1(SendCommand, std::string(const std::string&));
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
  mock_ = static_cast<MockATEInteraction*>(application_context_->ate_interaction_.get());
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
TEST_F(ApplicationContextTest, SendCommand_IsRunningTrue_SendCommandCallsOnce) {
  const std::string kCommand = "command";
  
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(true));
  EXPECT_CALL(*mock_, SendCommand(kCommand)).Times(1);

  application_context_->SendCommand(kCommand);
}

TEST_F(ApplicationContextTest, SendCommand_IsRunningFalse_Failure) {
  EXPECT_CALL(*mock_, IsConnectionOpened()).WillOnce(Return(false));
  EXPECT_CALL(*mock_, SendCommand(_)).Times(0);

  const std::string kCommand = "command";

  EXPECT_THROW(application_context_->SendCommand(kCommand), std::runtime_error);
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ate.h"
#include "ate_message_adapter.h"

// Helper macros for testing private section
#define private public
#define protected public
#include "interaction/DBUS/dbus_connection_manager.h"

namespace {

using ::testing::_;  // Matcher for parameters
using ::testing::Return;

class MockAteMessageAdapter : public IMessageAdapter {
 public:
  MOCK_METHOD1(OnMessage, std::string(const std::string& message));
};

class DBusConnectionManagerTest : public ::testing::Test {
 protected:
  void SetUp() override;

  const std::string kInterface{"com.luxoft.vdp.test.interface"};
  std::unique_ptr<interaction::DBusConnectionManager> manager_;
  MockAteMessageAdapter mock_message_adapter_;
};

void DBusConnectionManagerTest::SetUp() {
  manager_ = std::make_unique<interaction::DBusConnectionManager>(kInterface, mock_message_adapter_);
}

TEST_F(DBusConnectionManagerTest, CreateGlibLoop_Success) {
  ASSERT_TRUE(manager_->loop_ != nullptr) << "Glib loop for DBus polling not created.";
}

TEST_F(DBusConnectionManagerTest, StartStop_Success) {
  manager_->Start();
  EXPECT_TRUE(manager_->dbus_thread_.joinable());

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  manager_->Stop();
  EXPECT_FALSE(manager_->dbus_thread_.joinable());
}

TEST_F(DBusConnectionManagerTest, GenCorrelationId_Success) {
  EXPECT_EQ(1, manager_->GenCorrelationId());
  EXPECT_EQ(2, manager_->GenCorrelationId());
  EXPECT_EQ(3, manager_->GenCorrelationId());
}

}  // namespace

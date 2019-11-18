#include <atomic>
#include <csignal>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ate_message_adapter.h"

// Helper macros for testing private section
#define private public
#define protected public
#include "interaction/ATE/sig_connection_manager.h"

namespace {

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

class MockAteMessageAdapter : public IMessageAdapter {
 public:
  MOCK_METHOD1(OnMessage, std::string(const std::string& message));
};

class SigConnectionManagerTest : public ::testing::Test {
 protected:
  void SetUp() override;

  std::unique_ptr<interaction::SigConnectionManager> sig_manager_;
  boost::asio::io_context io_context_;
  MockAteMessageAdapter mock_ate_message_adapter_;
};

void SigConnectionManagerTest::SetUp() {
  sig_manager_ = std::make_unique<interaction::SigConnectionManager>(io_context_, mock_ate_message_adapter_);
}

TEST_F(SigConnectionManagerTest, StartStop_NoSignals_Success) {
  sig_manager_->Start();
  EXPECT_TRUE(sig_manager_->running_);
  EXPECT_TRUE(sig_manager_->loop_.joinable());

  std::this_thread::sleep_for(std::chrono::microseconds(100));
  sig_manager_->Stop();
  EXPECT_FALSE(sig_manager_->running_);
  EXPECT_FALSE(sig_manager_->loop_.joinable());
}

struct CondSig {
  std::string Wait(const std::string&) {
    signal_caught.exchange(true);
    return "true";
  }
  volatile std::atomic_bool signal_caught{false};
};

TEST_F(SigConnectionManagerTest, ReloadStorage_SendSIGHUP_Caught) {
  const auto kTimeSleep = 100u;
  CondSig condsig;

  sig_manager_->Start();

  EXPECT_CALL(mock_ate_message_adapter_, OnMessage(_)).Times(1).WillOnce(Invoke(&condsig, &CondSig::Wait));
  raise(SIGHUP);

  for (auto i = 0; !condsig.signal_caught && i < 12; ++i) {  // wait for signal nearly 1200ms, thread worker sleeps 1s
    std::this_thread::sleep_for(std::chrono::milliseconds(kTimeSleep));
  }
  EXPECT_TRUE(condsig.signal_caught) << "Signal wasn't caught";
}
}  // namespace

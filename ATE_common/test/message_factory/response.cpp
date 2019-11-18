#include <gtest/gtest.h>

#include "message_factory/message_factory.h"

namespace {
TEST(RpcResponseTest, CreateIconReloadResponse_EmptyResponse_EmptyResponse) {
  auto params = common::jmsg::MessageFactory::SignalConnection::CreateIconReloadResponse();
  EXPECT_FALSE(params.empty()) << "must be filled";
  EXPECT_TRUE(params.isBool());
  EXPECT_TRUE(params.asBool());
}
}  // namespace

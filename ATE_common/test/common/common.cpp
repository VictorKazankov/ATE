#include <thread>

#include <gtest/gtest.h>

#include "common.h"

namespace {

TEST(CommonTest, GetConfigMtTest) {
  auto get_config = []() { EXPECT_NO_THROW(common::Config()); };

  std::thread t1{get_config};
  std::thread t2{get_config};

  t1.join();
  t2.join();
}

TEST(CommonTest, SetUpLogger) { EXPECT_NO_THROW(common::SetUpLogger()); }

TEST(CommonTest, SetUpLoggerDup) { EXPECT_NO_THROW(common::SetUpLogger()); }

}  // namespace

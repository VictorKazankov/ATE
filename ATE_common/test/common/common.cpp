#include <thread>

#include <gtest/gtest.h>

#include "common.h"
#include "config/config.h"
#include "exceptions.h"

namespace {

TEST(CommonTest, SetUpAndDupeSetUpTest) {
  // Cannot split the test, as test run order is not guaranteed
  const std::string kDefaultConfigPath = ATE_COMMON_TEST_DATA_PATH "/test_config.ini";
  EXPECT_NO_THROW(common::SetUp(kDefaultConfigPath));
  EXPECT_NO_THROW(common::Config());
  EXPECT_NO_THROW(common::SetUp(kDefaultConfigPath));
}

}  // namespace

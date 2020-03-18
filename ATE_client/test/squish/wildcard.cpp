#include <gtest/gtest.h>

#define private public

#include "squish/wildcard.h"

namespace {
using namespace squish;

const auto kName = "name";
const auto kSyncVersion = "sync3";
const auto kBuildVersion = "222";
const auto kParentScreen = "home";
const auto kDayMode = common::squish::CollectionMode::kDay;

TEST(WildcardTest, WildcardConstructor_OnlyName_NameFilledModeIsNoneOtherFieldsEmpty) {
  Wildcard wildcard(kName);
  EXPECT_EQ(wildcard.name, kName);
  EXPECT_TRUE(wildcard.sync_version.empty());
  EXPECT_TRUE(wildcard.build_version.empty());
  EXPECT_TRUE(wildcard.parent_screen.empty());
  EXPECT_EQ(wildcard.mode, common::squish::CollectionMode::kNone);
  EXPECT_TRUE(wildcard.match_objects_.empty());
}

TEST(WildcardTest, WildcardConstructor_AllArgs_AllFieldsMatch) {
  Wildcard wildcard(kName, kSyncVersion, kBuildVersion, kParentScreen, kDayMode);
  EXPECT_EQ(wildcard.name, kName);
  EXPECT_EQ(wildcard.sync_version, kSyncVersion);
  EXPECT_EQ(wildcard.build_version, kBuildVersion);
  EXPECT_EQ(wildcard.parent_screen, kParentScreen);
  EXPECT_EQ(wildcard.mode, kDayMode);
  EXPECT_TRUE(wildcard.match_objects_.empty());
}
}  // namespace

#undef private

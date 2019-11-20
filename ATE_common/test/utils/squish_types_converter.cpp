#include "utils/squish_types_converter.h"

#include <gtest/gtest.h>

TEST(CollectionModeToStrTest, CollectionModeToStr_CorrectCollectionMode_Success) {
  auto mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::DAY);
  const auto day_mode = "day_mode";
  const auto night_mode = "night_mode";
  EXPECT_STREQ(mode.c_str(), day_mode) << "Mode: " << mode << " Expected mode: " << day_mode;

  mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::NIGHT);
  EXPECT_STREQ(mode.c_str(), night_mode) << "Mode: " << mode << " Expected mode: " << night_mode;
}

TEST(CollectionModeToStrTest, CollectionModeToStr_WrongCollectionMode_Failure) {
  auto mode = common::squish::CollectionModeToStr(static_cast<common::squish::CollectionMode>(-1));
  EXPECT_TRUE(mode.empty());
}

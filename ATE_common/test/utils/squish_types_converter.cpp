#include "utils/squish_types_converter.h"

#include <gtest/gtest.h>

TEST(CollectionModeToStrTest, CollectionModeToStr_CorrectCollectionMode_Success) {
  auto mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::DAY);
  const auto none_mode = "NONE";
  const auto any_mode = "ANY";
  const auto day_mode = "DAY";
  const auto night_mode = "NIGHT";

  EXPECT_STREQ(mode.c_str(), day_mode) << "Mode: " << mode << " Expected mode: " << day_mode;

  mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::NIGHT);
  EXPECT_STREQ(mode.c_str(), night_mode) << "Mode: " << mode << " Expected mode: " << night_mode;

  mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::NONE);
  EXPECT_STREQ(mode.c_str(), none_mode) << "Mode: " << mode << " Expected mode: " << none_mode;

  mode = common::squish::CollectionModeToStr(common::squish::CollectionMode::ANY);
  EXPECT_STREQ(mode.c_str(), any_mode) << "Mode: " << mode << " Expected mode: " << any_mode;
}

TEST(CollectionModeToStrTest, CollectionModeToStr_WrongCollectionMode_Failure) {
  auto mode = common::squish::CollectionModeToStr(static_cast<common::squish::CollectionMode>(-1));
  EXPECT_TRUE(mode.empty());
}

TEST(StrToCollectionModeTest, StrToCollectionMode_CorrectCollectionMode_Success) {
  using namespace common::squish;
  const std::pair<std::string, CollectionMode> none_mode = {"NONE", CollectionMode::NONE};
  const std::pair<std::string, CollectionMode> any_mode = {"ANY", CollectionMode::ANY};
  const std::pair<std::string, CollectionMode> day_mode = {"DAY", CollectionMode::DAY};
  const std::pair<std::string, CollectionMode> night_mode = {"NIGHT", CollectionMode::NIGHT};

  auto mode = StrToCollectionMode(none_mode.first);
  EXPECT_EQ(mode, none_mode.second) << "Mode: " << CollectionModeToStr(mode)
                                    << " Expected mode: " << CollectionModeToStr(none_mode.second);

  mode = StrToCollectionMode(any_mode.first);
  EXPECT_EQ(mode, any_mode.second) << "Mode: " << CollectionModeToStr(mode)
                                   << " Expected mode: " << CollectionModeToStr(any_mode.second);

  mode = StrToCollectionMode(day_mode.first);
  EXPECT_EQ(mode, day_mode.second) << "Mode: " << CollectionModeToStr(mode)
                                   << " Expected mode: " << CollectionModeToStr(day_mode.second);

  mode = StrToCollectionMode(night_mode.first);
  EXPECT_EQ(mode, night_mode.second) << "Mode: " << CollectionModeToStr(mode)
                                     << " Expected mode: " << CollectionModeToStr(night_mode.second);
}

TEST(StrToCollectionModeTest, StrToCollectionMode_WrongCollectionMode_Failure) {
  EXPECT_THROW(common::squish::StrToCollectionMode("some_value"), std::runtime_error);
}
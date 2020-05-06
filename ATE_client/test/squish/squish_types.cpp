#include <gtest/gtest.h>

#include "error_defines.h"

#define private public
#define protected public
#include "squish/squish_types.h"

namespace {
using namespace squish;

TEST(ObjectTest, ObjectConstructor_NoArgs_ObjEmpty) {
  Object obj;
  EXPECT_EQ(obj.x, 0);
  EXPECT_EQ(obj.y, 0);
  EXPECT_EQ(obj.width, 0);
  EXPECT_EQ(obj.height, 0);
  EXPECT_TRUE(obj.sync_version.empty());
  EXPECT_TRUE(obj.build_version.empty());
  EXPECT_EQ(obj.mode, common::squish::CollectionMode::kNone);
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.parent_screen.empty());
}

TEST(ObjectTest, ObjectConstructor_XY_Success) {
  Object obj{10, 20};
  EXPECT_EQ(obj.x, 10);
  EXPECT_EQ(obj.y, 20);
  EXPECT_EQ(obj.width, 0);
  EXPECT_EQ(obj.height, 0);
  EXPECT_TRUE(obj.sync_version.empty());
  EXPECT_TRUE(obj.build_version.empty());
  EXPECT_EQ(obj.mode, common::squish::CollectionMode::kNone);
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.parent_screen.empty());
}

TEST(ObjectTest, ObjectConstructor_XYWH_Success) {
  Object obj{10, 20, 100, 200};
  EXPECT_EQ(obj.x, 10);
  EXPECT_EQ(obj.y, 20);
  EXPECT_EQ(obj.width, 100);
  EXPECT_EQ(obj.height, 200);
  EXPECT_TRUE(obj.sync_version.empty());
  EXPECT_TRUE(obj.build_version.empty());
  EXPECT_EQ(obj.mode, common::squish::CollectionMode::kNone);
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.parent_screen.empty());
}

TEST(ObjectTest, TopLeft_DefaultCornerCoordinates_ExpectedCoordinates) {
  Object obj;
  auto top_left_coordinate = obj.TopLeft();
  EXPECT_EQ(obj.x_top_left, top_left_coordinate.x);
  EXPECT_EQ(obj.y_top_left, top_left_coordinate.y);
}

TEST(ObjectTest, BottomRight_DefaultCornerCoordinates_ExpectedCoordinates) {
  Object obj;
  auto bottom_right_coordinate = obj.BottomRight();
  EXPECT_EQ(obj.x_bottom_right, bottom_right_coordinate.x);
  EXPECT_EQ(obj.y_bottom_right, bottom_right_coordinate.y);
}

}  // namespace

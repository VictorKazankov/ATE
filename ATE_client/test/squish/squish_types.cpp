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
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.type.empty());
}

TEST(ObjectTest, ObjectConstructor_XY_Success) {
  Object obj{10, 20};
  EXPECT_EQ(obj.x, 10);
  EXPECT_EQ(obj.y, 20);
  EXPECT_EQ(obj.width, 0);
  EXPECT_EQ(obj.height, 0);
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.type.empty());
}

TEST(ObjectTest, ObjectConstructor_XYWH_Success) {
  Object obj{10, 20, 100, 200};
  EXPECT_EQ(obj.x, 10);
  EXPECT_EQ(obj.y, 20);
  EXPECT_EQ(obj.width, 100);
  EXPECT_EQ(obj.height, 200);
  EXPECT_TRUE(obj.name.empty());
  EXPECT_TRUE(obj.type.empty());
}

TEST(ObjectTest, Exists_ConnectionNotEstablished_Failure) {
  Object obj;
  EXPECT_FALSE(obj.Exists("some_unknown_object"));
}
}  // namespace

#include "utils/geometry_types.h"

#include <gtest/gtest.h>

namespace {

constexpr auto kAgregateConstructing = "Problem with agregate constructing";
constexpr auto kCopying = "Problem with copying";
constexpr auto kAssigning = "Problem with assigning";
constexpr auto kTemplateConstructing = "Problem with template constructing";
constexpr auto kTemplateAssigning = "Problem with template assigning";
constexpr auto kConversion = "Problem with conversion to other type";

template <typename Number>
struct TestPoint {
  Number x;
  Number y;
};

template <typename Number>
struct TestRect {
  Number x;
  Number y;
  Number width;
  Number height;
};

TEST(FacilitiesTest, TestPoint) {
  common::Point a{1, 2};

  EXPECT_EQ(a.x, 1) << kAgregateConstructing;
  EXPECT_EQ(a.y, 2) << kAgregateConstructing;

  common::Point b = a;

  EXPECT_EQ(a.x, b.x) << kCopying;
  EXPECT_EQ(a.y, b.y) << kCopying;

  common::Point c;
  c = a;

  EXPECT_EQ(a.x, c.x) << kAssigning;
  EXPECT_EQ(a.y, c.y) << kAssigning;

  TestPoint<int> tp{3, 4};
  common::Point d = tp;

  EXPECT_EQ(d.x, tp.x) << kTemplateConstructing;
  EXPECT_EQ(d.y, tp.y) << kTemplateConstructing;

  b = tp;

  EXPECT_EQ(b.x, tp.x) << kTemplateAssigning;
  EXPECT_EQ(b.y, tp.y) << kTemplateAssigning;

  tp = static_cast<TestPoint<int>>(common::Point{5, 6});

  EXPECT_EQ(tp.x, 5) << kConversion;
  EXPECT_EQ(tp.y, 6) << kConversion;
}

TEST(FacilitiesTest, TestRect) {
  common::Rect a{1, 2, 3, 4};

  EXPECT_EQ(a.x, 1) << kAgregateConstructing;
  EXPECT_EQ(a.y, 2) << kAgregateConstructing;
  EXPECT_EQ(a.width, 3) << kAgregateConstructing;
  EXPECT_EQ(a.height, 4) << kAgregateConstructing;

  common::Rect b = a;

  EXPECT_EQ(a.x, b.x) << kCopying;
  EXPECT_EQ(a.y, b.y) << kCopying;
  EXPECT_EQ(a.width, b.width) << kCopying;
  EXPECT_EQ(a.height, b.height) << kCopying;

  common::Rect c;
  c = a;

  EXPECT_EQ(a.x, c.x) << kAssigning;
  EXPECT_EQ(a.y, c.y) << kAssigning;
  EXPECT_EQ(a.width, c.width) << kAssigning;
  EXPECT_EQ(a.height, c.height) << kAssigning;

  TestRect<int> tr{5, 6, 7, 8};
  common::Rect d = tr;

  EXPECT_EQ(d.x, tr.x) << kTemplateConstructing;
  EXPECT_EQ(d.y, tr.y) << kTemplateConstructing;
  EXPECT_EQ(d.width, tr.width) << kTemplateConstructing;
  EXPECT_EQ(d.height, tr.height) << kTemplateConstructing;

  b = tr;

  EXPECT_EQ(b.x, tr.x) << kTemplateAssigning;
  EXPECT_EQ(b.y, tr.y) << kTemplateAssigning;
  EXPECT_EQ(b.width, tr.width) << kTemplateAssigning;
  EXPECT_EQ(b.height, tr.height) << kTemplateAssigning;

  tr = static_cast<TestRect<int>>(common::Rect{9, 10, 11, 12});

  EXPECT_EQ(tr.x, 9) << kConversion;
  EXPECT_EQ(tr.y, 10) << kConversion;
  EXPECT_EQ(tr.width, 11) << kConversion;
  EXPECT_EQ(tr.height, 12) << kConversion;

  common::Rect rect{10, 20, 10, 10};
  common::Point point{15, 25};

  EXPECT_EQ(rect.Center().x, point.x);
  EXPECT_EQ(rect.Center().y, point.y);
}

}  // namespace

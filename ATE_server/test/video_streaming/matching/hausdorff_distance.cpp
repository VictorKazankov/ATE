#include "video_streaming/matching/hausdorff_distance.h"

#include <array>

#include <gtest/gtest.h>

#include <opencv2/core/core.hpp>

namespace {

using detector::HausdorffDistance;
using detector::detail::LinearHausdorffDistance;

constexpr auto kIdentityMessage = "The distance between the same objects must be equal to 0";
constexpr auto kSymmetryMessage = "The distance must be symmetrical";
constexpr auto kTriangleInequalityMessage =
    "The direct distance must be less than the distance through the intermediary";

TEST(HausdorffDistance, IdentitySingleDimension) {
  const std::array<int, 2> s{-1, 2};
  const int d = LinearHausdorffDistance(s, s);
  EXPECT_EQ(d, 0) << kIdentityMessage;
}

TEST(HausdorffDistance, SymmetrySingleDimension) {
  const std::array<int, 2> s1{1, 2};
  const std::array<int, 2> s2{5, 8};

  const int d12 = LinearHausdorffDistance(s1, s2);
  const int d21 = LinearHausdorffDistance(s2, s1);

  EXPECT_EQ(d12, d21) << kSymmetryMessage;
}

TEST(HausdorffDistance, TriangleInequalitySingleDimension) {
  const std::array<int, 2> s1{-2, 3};
  const std::array<int, 2> s2{1, 8};
  const std::array<int, 2> s3{4, 5};

  const int d12 = LinearHausdorffDistance(s1, s2);
  const int d13 = LinearHausdorffDistance(s1, s3);
  const int d23 = LinearHausdorffDistance(s2, s3);

  EXPECT_LE(d12, d13 + d23) << kTriangleInequalityMessage;
  EXPECT_LE(d13, d12 + d23) << kTriangleInequalityMessage;
  EXPECT_LE(d23, d12 + d13) << kTriangleInequalityMessage;
}

TEST(HausdorffDistance, Identity) {
  const cv::Rect r{0, -1, 5, 2};
  const double d = HausdorffDistance(r, r);
  EXPECT_DOUBLE_EQ(d, 0) << kIdentityMessage;
}

TEST(HausdorffDistance, Symmetry) {
  const cv::Rect r1{0, 0, 3, 4};
  const cv::Rect r2{-7, 4, 1, 2};

  const double d12 = HausdorffDistance(r1, r2);
  const double d21 = HausdorffDistance(r2, r1);

  EXPECT_DOUBLE_EQ(d12, d21) << kSymmetryMessage;
}

TEST(HausdorffDistance, TriangleInequality) {
  const cv::Rect r1{1, 2, 3, 4};
  const cv::Rect r2{5, 6, 7, 8};
  const cv::Rect r3{9, 10, 11, 12};

  const double d12 = HausdorffDistance(r1, r2);
  const double d13 = HausdorffDistance(r1, r3);
  const double d23 = HausdorffDistance(r2, r3);

  EXPECT_LE(d12, d13 + d23) << kTriangleInequalityMessage;
  EXPECT_LE(d13, d12 + d23) << kTriangleInequalityMessage;
  EXPECT_LE(d23, d12 + d13) << kTriangleInequalityMessage;
}

}  // namespace

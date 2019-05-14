#ifndef ATE_SERVER_UTILS_HAUSDORFF_DISTANCE_H_
#define ATE_SERVER_UTILS_HAUSDORFF_DISTANCE_H_

#include <array>
#include <cmath>
#include <cstdlib>
#include <limits>

namespace detector {

namespace detail {

/**
 * @brief Calculate Hausdorff distance between linear sections
 */
template <typename SignedType>
SignedType LinearHausdorffDistance(const std::array<SignedType, 2>& s1, const std::array<SignedType, 2>& s2) {
  static_assert(std::numeric_limits<SignedType>::is_signed, "Works only with a signed type");

  /**
   * Calculation scheme:
   *
   * d(i, j) = abs(s1(i), s2(j))
   *
   * dist[0] = d(0, 0)
   * dist[1] = d(0, 1)
   * dist[2] = d(1, 0)
   * dist[3] = d(1, 1)
   *
   * LinearHausdorffDistance = min(max(dist[0], dist[3]), max(max(dist[1], dist[2])))
   */
  std::array<SignedType, 4> dist;
  auto dist_iter = dist.begin();
  for (const SignedType& v1 : s1) {
    for (const SignedType& v2 : s2) {
      *dist_iter++ = std::abs(v1 - v2);
    }
  }

  return std::min(std::max(std::get<0>(dist), std::get<3>(dist)), std::max(std::get<1>(dist), std::get<2>(dist)));
}

}  // namespace detail

/**
 * @brief Calculate Hausdorff distance between rectangles
 */
template <typename Rectangle>
auto HausdorffDistance(const Rectangle& r1, const Rectangle& r2) {
  using value_type = typename Rectangle::value_type;

  /**
   * Reducing to x and y components
   */
  const auto top_left1 = r1.tl();
  const auto top_left2 = r2.tl();
  const auto bottom_right1 = r1.br();
  const auto bottom_right2 = r2.br();

  const std::array<value_type, 2> x1 = {bottom_right1.x, top_left1.x};
  const std::array<value_type, 2> x2 = {bottom_right2.x, top_left2.x};
  const std::array<value_type, 2> y1 = {bottom_right1.y, top_left1.y};
  const std::array<value_type, 2> y2 = {bottom_right2.y, top_left2.y};

  return std::hypot(detail::LinearHausdorffDistance(x1, x2), detail::LinearHausdorffDistance(y1, y2));
}

}  // namespace detector

#endif  // ATE_SERVER_UTILS_HAUSDORFF_DISTANCE_H_

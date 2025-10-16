/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Modernized C++ implementation of libccd (Continuous Collision Detection)
 * Based on libccd by Daniel Fiser <danfis@danfis.cz>
 */

#pragma once

#include <Eigen/Core>

namespace dart::collision::ccd {

//==============================================================================
/// Support point in Minkowski difference
/// Corresponds to ccd_support_t in the original libccd
template <typename S>
struct SupportPoint
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 v;  ///< Support point in Minkowski sum (v1 - v2)
  Vector3 v1; ///< Support point in obj1
  Vector3 v2; ///< Support point in obj2

  SupportPoint() = default;

  SupportPoint(const Vector3& v_, const Vector3& v1_, const Vector3& v2_)
    : v(v_), v1(v1_), v2(v2_)
  {
  }
};

} // namespace dart::collision::ccd

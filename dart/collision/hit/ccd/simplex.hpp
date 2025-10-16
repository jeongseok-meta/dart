/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Modernized C++ implementation of libccd (Continuous Collision Detection)
 * Based on libccd by Daniel Fiser <danfis@danfis.cz>
 */

#pragma once

#include "support.hpp"

#include <algorithm>
#include <array>

namespace dart::collision::ccd {

//==============================================================================
/// Simplex for GJK algorithm (up to 4 points in 3D)
/// Corresponds to ccd_simplex_t in the original libccd
template <typename S>
class Simplex
{
public:
  using SupportPointT = SupportPoint<S>;

  Simplex() : mSize(0) {}

  /// Add a support point to the simplex
  void add(const SupportPointT& point)
  {
    if (mSize < 4) {
      mPoints[mSize++] = point;
    }
  }

  /// Set support point at given index
  void set(size_t idx, const SupportPointT& point)
  {
    mPoints[idx] = point;
  }

  /// Get support point at given index (const)
  const SupportPointT& operator[](size_t idx) const
  {
    return mPoints[idx];
  }

  /// Get support point at given index (non-const)
  SupportPointT& operator[](size_t idx)
  {
    return mPoints[idx];
  }

  /// Get the last added support point (const)
  const SupportPointT& last() const
  {
    return mPoints[mSize - 1];
  }

  /// Get the last added support point (non-const)
  SupportPointT& last()
  {
    return mPoints[mSize - 1];
  }

  /// Get the number of points in the simplex
  size_t size() const
  {
    return mSize;
  }

  /// Set the size of the simplex (used to reduce simplex)
  void setSize(size_t size)
  {
    mSize = size;
  }

  /// Swap two points in the simplex
  void swap(size_t i, size_t j)
  {
    std::swap(mPoints[i], mPoints[j]);
  }

private:
  std::array<SupportPointT, 4> mPoints;
  size_t mSize;
};

} // namespace dart::collision::ccd

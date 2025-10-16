// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Modern C++ support functions for DART shapes in CCD collision detection
 */

#pragma once

#include "ccd.hpp"

#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/CapsuleShape.hpp>
#include <dart/dynamics/SphereShape.hpp>

#include <Eigen/Core>

namespace dart::collision::ccd {

//==============================================================================
/// Sphere support function
//==============================================================================
template <typename S>
void sphereSupport(
    const void* obj,
    const Eigen::Vector3<S>& dir,
    Eigen::Vector3<S>& support)
{
  const auto* sphere = static_cast<const dynamics::SphereShape*>(obj);
  S radius = static_cast<S>(sphere->getRadius());

  S len = dir.norm();
  if (len > S(1e-10)) {
    support = (dir / len) * radius;
  } else {
    support.setZero();
    support[0] = radius;
  }
}

//==============================================================================
/// Box support function
//==============================================================================
template <typename S>
void boxSupport(
    const void* obj,
    const Eigen::Vector3<S>& dir,
    Eigen::Vector3<S>& support)
{
  const auto* box = static_cast<const dynamics::BoxShape*>(obj);
  const Eigen::Vector3d& size = box->getSize();

  // Support point is at the corner in the direction of dir
  support[0] = (dir[0] >= S(0)) ? S(size[0] * 0.5) : S(-size[0] * 0.5);
  support[1] = (dir[1] >= S(0)) ? S(size[1] * 0.5) : S(-size[1] * 0.5);
  support[2] = (dir[2] >= S(0)) ? S(size[2] * 0.5) : S(-size[2] * 0.5);
}

//==============================================================================
/// Capsule support function
//==============================================================================
template <typename S>
void capsuleSupport(
    const void* obj,
    const Eigen::Vector3<S>& dir,
    Eigen::Vector3<S>& support)
{
  const auto* capsule = static_cast<const dynamics::CapsuleShape*>(obj);
  S radius = static_cast<S>(capsule->getRadius());
  S height = static_cast<S>(capsule->getHeight());
  S halfHeight = height * S(0.5);

  // Capsule is oriented along Z-axis
  // First find which hemisphere (top or bottom)
  S dirZ = dir[2];
  S signZ = (dirZ >= S(0)) ? S(1) : S(-1);

  // Center of the hemisphere
  Eigen::Vector3<S> center;
  center.setZero();
  center[2] = signZ * halfHeight;

  // Support point on the hemisphere
  S len = dir.norm();
  if (len > S(1e-10)) {
    support = center + (dir / len) * radius;
  } else {
    support = center;
    support[0] = radius;
  }
}

//==============================================================================
/// Center function for shapes
//==============================================================================
template <typename S>
void shapeCenter(const void* /*obj*/, Eigen::Vector3<S>& center)
{
  center.setZero();
}

//==============================================================================
/// Create CCD configuration for a given shape
//==============================================================================
template <typename S>
void configureCcdForShape(
    const dynamics::Shape* shape,
    CcdConfig<S>& config)
{
  const std::string& type = shape->getType();

  if (type == dynamics::SphereShape::getStaticType()) {
    config.support1 = sphereSupport<S>;
    config.support2 = sphereSupport<S>;
  } else if (type == dynamics::BoxShape::getStaticType()) {
    config.support1 = boxSupport<S>;
    config.support2 = boxSupport<S>;
  } else if (type == dynamics::CapsuleShape::getStaticType()) {
    config.support1 = capsuleSupport<S>;
    config.support2 = capsuleSupport<S>;
  } else {
    // Default to sphere support for unsupported shapes
    config.support1 = sphereSupport<S>;
    config.support2 = sphereSupport<S>;
  }

  config.center1 = shapeCenter<S>;
  config.center2 = shapeCenter<S>;
}

} // namespace dart::collision::ccd

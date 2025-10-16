/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2024, DART Development Team
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/// @brief Convenience header for HIT collision detection library
///
/// HIT (Hybrid Incremental Trees) is DART's integrated collision detection
/// library based on the Flexible Collision Library (FCL).
///
/// This header provides access to the core HIT collision detection
/// functionality. For DART integration, use the adapter classes:
///   - dart::collision::HitCollisionDetector
///   - dart::collision::HitCollisionGroup
///   - dart::collision::HitCollisionObject
///
/// To use HIT directly (advanced):
///   #include "dart/collision/hit/narrowphase/collision.h"
///   #include "dart/collision/hit/broadphase/broadphase_dynamic_AABB_tree.h"

#pragma once

// Configuration
#include "dart/collision/hit/config.h"

// Core collision detection
#include "dart/collision/hit/narrowphase/collision.h"
#include "dart/collision/hit/narrowphase/collision_object.h"
#include "dart/collision/hit/narrowphase/collision_request.h"
#include "dart/collision/hit/narrowphase/collision_result.h"
// TEMPORARY: distance.h excluded due to missing traversal base class
// #include "dart/collision/hit/narrowphase/distance.h"
#include "dart/collision/hit/narrowphase/distance_request.h"
#include "dart/collision/hit/narrowphase/distance_result.h"

// Broadphase algorithms
#include "dart/collision/hit/broadphase/broadphase_collision_manager.h"
#include "dart/collision/hit/broadphase/broadphase_dynamic_AABB_tree.h"

// Geometry types
#include "dart/collision/hit/geometry/collision_geometry.h"
#include "dart/collision/hit/geometry/shape/box.h"
#include "dart/collision/hit/geometry/shape/capsule.h"
#include "dart/collision/hit/geometry/shape/cone.h"
#include "dart/collision/hit/geometry/shape/convex.h"
#include "dart/collision/hit/geometry/shape/cylinder.h"
#include "dart/collision/hit/geometry/shape/plane.h"
#include "dart/collision/hit/geometry/shape/sphere.h"

// Math utilities
#include "dart/collision/hit/common/types.h"
#include "dart/collision/hit/math/bv/AABB.h"

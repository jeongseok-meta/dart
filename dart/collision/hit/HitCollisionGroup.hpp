/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/main/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <dart/collision/CollisionGroup.hpp>
#include <dart/collision/hit/broadphase/broadphase_dynamic_AABB_tree.h>

namespace dart::collision {

class CollisionObject;
class HitCollisionObjectUserData;

/// Collision group implementation using HIT library
class HitCollisionGroup : public CollisionGroup
{
public:
  friend class HitCollisionDetector;

  using HitCollisionManager
      = dart::collision::hit::DynamicAABBTreeCollisionManager<double>;

  /// Constructor
  HitCollisionGroup(const CollisionDetectorPtr& collisionDetector);

  /// Destructor
  virtual ~HitCollisionGroup() = default;

protected:
  using CollisionGroup::updateEngineData;

  // Documentation inherited
  void initializeEngineData() override;

  // Documentation inherited
  void addCollisionObjectToEngine(CollisionObject* object) override;

  // Documentation inherited
  void addCollisionObjectsToEngine(
      const std::vector<CollisionObject*>& collObjects) override;

  // Documentation inherited
  void removeCollisionObjectFromEngine(CollisionObject* object) override;

  // Documentation inherited
  void removeAllCollisionObjectsFromEngine() override;

  // Documentation inherited
  void updateCollisionGroupEngineData() override;

  /// Return HIT collision manager that is also a broad-phase algorithm
  HitCollisionManager* getHitCollisionManager();

  /// Return HIT collision manager that is also a broad-phase algorithm
  const HitCollisionManager* getHitCollisionManager() const;

protected:
  /// HIT broad-phase algorithm
  std::unique_ptr<HitCollisionManager> mBroadPhaseAlg;
};

} // namespace dart::collision

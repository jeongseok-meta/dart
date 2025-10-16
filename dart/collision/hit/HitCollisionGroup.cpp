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

#include <dart/collision/CollisionObject.hpp>
#include <dart/collision/hit/HitCollisionGroup.hpp>
#include <dart/collision/hit/HitCollisionObject.hpp>

namespace dart::collision {

//==============================================================================
HitCollisionGroup::HitCollisionGroup(
    const CollisionDetectorPtr& collisionDetector)
  : CollisionGroup(collisionDetector),
    mBroadPhaseAlg(
        new dart::collision::hit::DynamicAABBTreeCollisionManager<double>())
{
  // Do nothing
}

//==============================================================================
void HitCollisionGroup::initializeEngineData()
{
  mBroadPhaseAlg->setup();
}

//==============================================================================
void HitCollisionGroup::addCollisionObjectToEngine(CollisionObject* object)
{
  auto casted = static_cast<HitCollisionObject*>(object);
  mBroadPhaseAlg->registerObject(casted->getHitCollisionObject());

  initializeEngineData();
}

//==============================================================================
void HitCollisionGroup::addCollisionObjectsToEngine(
    const std::vector<CollisionObject*>& collObjects)
{
  for (auto collObj : collObjects) {
    auto casted = static_cast<HitCollisionObject*>(collObj);

    mBroadPhaseAlg->registerObject(casted->getHitCollisionObject());
  }

  initializeEngineData();
}

//==============================================================================
void HitCollisionGroup::removeCollisionObjectFromEngine(CollisionObject* object)
{
  auto casted = static_cast<HitCollisionObject*>(object);

  mBroadPhaseAlg->unregisterObject(casted->getHitCollisionObject());

  initializeEngineData();
}

//==============================================================================
void HitCollisionGroup::removeAllCollisionObjectsFromEngine()
{
  mBroadPhaseAlg->clear();

  initializeEngineData();
}

//==============================================================================
void HitCollisionGroup::updateCollisionGroupEngineData()
{
  mBroadPhaseAlg->update();
}

//==============================================================================
HitCollisionGroup::HitCollisionManager*
HitCollisionGroup::getHitCollisionManager()
{
  return mBroadPhaseAlg.get();
}

//==============================================================================
const HitCollisionGroup::HitCollisionManager*
HitCollisionGroup::getHitCollisionManager() const
{
  return mBroadPhaseAlg.get();
}

} // namespace dart::collision

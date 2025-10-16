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
#include <dart/collision/hit/HitCollisionDetector.hpp>
#include <dart/collision/hit/HitCollisionGroup.hpp>
#include <dart/collision/hit/HitCollisionObject.hpp>
#include <dart/collision/hit/HitTypes.hpp>

#include <dart/dynamics/Shape.hpp>
#include <dart/dynamics/ShapeFrame.hpp>

#include <dart/common/Console.hpp>

namespace dart::collision {

//==============================================================================
HitCollisionDetector::Registrar<HitCollisionDetector>
    HitCollisionDetector::mRegistrar{
        HitCollisionDetector::getStaticType(),
        []() -> std::shared_ptr<HitCollisionDetector> {
          return HitCollisionDetector::create();
        }};

//==============================================================================
std::shared_ptr<HitCollisionDetector> HitCollisionDetector::create()
{
  return std::shared_ptr<HitCollisionDetector>(new HitCollisionDetector());
}

//==============================================================================
HitCollisionDetector::HitCollisionDetector()
  : CollisionDetector(),
    mPrimitiveShapeType(PRIMITIVE),
    mContactPointComputationMethod(DART)
{
  mCollisionObjectManager.reset(new ManagerForUnsharableCollisionObjects(this));
}

//==============================================================================
HitCollisionDetector::~HitCollisionDetector()
{
  assert(mShapeMap.empty());
}

//==============================================================================
std::shared_ptr<CollisionDetector>
HitCollisionDetector::cloneWithoutCollisionObjects() const
{
  return HitCollisionDetector::create();
}

//==============================================================================
const std::string& HitCollisionDetector::getType() const
{
  return getStaticType();
}

//==============================================================================
const std::string& HitCollisionDetector::getStaticType()
{
  static const std::string type = "hit";
  return type;
}

//==============================================================================
std::unique_ptr<CollisionGroup> HitCollisionDetector::createCollisionGroup()
{
  return std::make_unique<HitCollisionGroup>(shared_from_this());
}

//==============================================================================
bool HitCollisionDetector::collide(
    CollisionGroup* /*group*/,
    const CollisionOption& option,
    CollisionResult* result)
{
  if (result)
    result->clear();

  if (0u == option.maxNumContacts)
    return false;

  dtwarn << "[HitCollisionDetector::collide] Not yet implemented\n";
  return false;
}

//==============================================================================
bool HitCollisionDetector::collide(
    CollisionGroup* /*group1*/,
    CollisionGroup* /*group2*/,
    const CollisionOption& option,
    CollisionResult* result)
{
  if (result)
    result->clear();

  if (0u == option.maxNumContacts)
    return false;

  dtwarn << "[HitCollisionDetector::collide] Not yet implemented\n";
  return false;
}

//==============================================================================
double HitCollisionDetector::distance(
    CollisionGroup* /*group*/,
    const DistanceOption& /*option*/,
    DistanceResult* result)
{
  if (result)
    result->clear();

  dtwarn << "[HitCollisionDetector::distance] Not yet implemented\n";
  return 0.0;
}

//==============================================================================
double HitCollisionDetector::distance(
    CollisionGroup* /*group1*/,
    CollisionGroup* /*group2*/,
    const DistanceOption& /*option*/,
    DistanceResult* result)
{
  if (result)
    result->clear();

  dtwarn << "[HitCollisionDetector::distance] Not yet implemented\n";
  return 0.0;
}

//==============================================================================
void HitCollisionDetector::setPrimitiveShapeType(PrimitiveShape type)
{
  mPrimitiveShapeType = type;
}

//==============================================================================
HitCollisionDetector::PrimitiveShape
HitCollisionDetector::getPrimitiveShapeType() const
{
  return mPrimitiveShapeType;
}

//==============================================================================
void HitCollisionDetector::setContactPointComputationMethod(
    ContactPointComputationMethod method)
{
  mContactPointComputationMethod = method;
}

//==============================================================================
HitCollisionDetector::ContactPointComputationMethod
HitCollisionDetector::getContactPointComputationMethod() const
{
  return mContactPointComputationMethod;
}

//==============================================================================
std::unique_ptr<CollisionObject> HitCollisionDetector::createCollisionObject(
    const dynamics::ShapeFrame* shapeFrame)
{
  auto shape = shapeFrame->getShape();
  auto hitCollGeom = claimHitCollisionGeometry(shape);

  if (!hitCollGeom) {
    dtwarn << "[HitCollisionDetector::createCollisionObject] Failed to create "
           << "collision geometry for shape type: " << shape->getType() << "\n";
    return nullptr;
  }

  auto collObj = std::unique_ptr<HitCollisionObject>(
      new HitCollisionObject(this, shapeFrame, hitCollGeom));

  return collObj;
}

//==============================================================================
void HitCollisionDetector::refreshCollisionObject(CollisionObject* object)
{
  auto casted = static_cast<HitCollisionObject*>(object);
  casted->updateEngineData();
}

//==============================================================================
std::shared_ptr<dart::collision::hit::CollisionGeometry<double>>
HitCollisionDetector::claimHitCollisionGeometry(
    const dynamics::ConstShapePtr& shape)
{
  auto search = mShapeMap.find(shape);

  if (search != mShapeMap.end()) {
    auto& shapeInfo = search->second;

    if (shapeInfo.mLastKnownVersion != shape->getVersion()) {
      auto hitCollGeom = createHitCollisionGeometry(
          shape, mPrimitiveShapeType, HitCollisionGeometryDeleter(this, shape));

      shapeInfo.mShape = hitCollGeom;
      shapeInfo.mLastKnownVersion = shape->getVersion();
    }

    auto& weakPtr = shapeInfo.mShape;

    if (auto spt = weakPtr.lock())
      return spt;
    else
      mShapeMap.erase(search);
  }

  auto hitCollGeom = createHitCollisionGeometry(
      shape, mPrimitiveShapeType, HitCollisionGeometryDeleter(this, shape));

  mShapeMap[shape] = ShapeInfo{hitCollGeom, shape->getVersion()};

  return hitCollGeom;
}

//==============================================================================
std::shared_ptr<dart::collision::hit::CollisionGeometry<double>>
HitCollisionDetector::createHitCollisionGeometry(
    const dynamics::ConstShapePtr& shape,
    HitCollisionDetector::PrimitiveShape /*type*/,
    const HitCollisionGeometryDeleter& /*deleter*/)
{
  dtwarn << "[HitCollisionDetector::createHitCollisionGeometry] Shape "
         << "conversion not yet implemented for type: " << shape->getType()
         << "\n";
  return nullptr;
}

//==============================================================================
HitCollisionDetector::HitCollisionGeometryDeleter::HitCollisionGeometryDeleter(
    HitCollisionDetector* cd, const dynamics::ConstShapePtr& shape)
  : mHitCollisionDetector(cd), mShape(shape)
{
  // Do nothing
}

//==============================================================================
void HitCollisionDetector::HitCollisionGeometryDeleter::operator()(
    dart::collision::hit::CollisionGeometry<double>* geom) const
{
  mHitCollisionDetector->mShapeMap.erase(mShape);
  delete geom;
}

} // namespace dart::collision

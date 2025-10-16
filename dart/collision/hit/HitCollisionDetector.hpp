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

#include <dart/collision/CollisionDetector.hpp>
#include <dart/collision/hit/HitTypes.hpp>

#include <vector>

namespace dart::collision {

class HitCollisionObject;

/// Collision detector using the HIT (Hybrid Incremental Trees) library.
///
/// HIT is DART's integrated collision detection library based on the
/// Flexible Collision Library (FCL). It provides efficient collision
/// and distance queries using hierarchical bounding volume trees.
class HitCollisionDetector : public CollisionDetector
{
public:
  using CollisionDetector::createCollisionGroup;

  /// Create a new HitCollisionDetector
  static std::shared_ptr<HitCollisionDetector> create();

  /// Whether to use analytic collision checking for primitive shapes.
  ///
  /// PRIMITIVE: Use HIT's analytic collision checking for primitive shapes.
  /// MESH: Use approximate mesh shapes for the primitive shapes.
  ///
  /// Note: Primitive shape collision returns more accurate contact information
  /// for simple shapes (box, sphere, cylinder, capsule, cone).
  enum PrimitiveShape
  {
    PRIMITIVE = 0,
    MESH
  };

  /// Whether to use HIT's contact point computation.
  ///
  /// HIT: Use HIT's contact point computation.
  /// DART: Use DART's own contact point computation
  enum ContactPointComputationMethod
  {
    HIT = 0,
    DART
  };

  /// Destructor
  virtual ~HitCollisionDetector();

  // Documentation inherited
  std::shared_ptr<CollisionDetector> cloneWithoutCollisionObjects()
      const override;

  // Documentation inherited
  const std::string& getType() const override;

  /// Get collision detector type for this class
  static const std::string& getStaticType();

  // Documentation inherited
  std::unique_ptr<CollisionGroup> createCollisionGroup() override;

  // Documentation inherited
  bool collide(
      CollisionGroup* group,
      const CollisionOption& option = CollisionOption(false, 1u, nullptr),
      CollisionResult* result = nullptr) override;

  // Documentation inherited
  bool collide(
      CollisionGroup* group1,
      CollisionGroup* group2,
      const CollisionOption& option = CollisionOption(false, 1u, nullptr),
      CollisionResult* result = nullptr) override;

  // Documentation inherited
  double distance(
      CollisionGroup* group,
      const DistanceOption& option = DistanceOption(false, 0.0, nullptr),
      DistanceResult* result = nullptr) override;

  // Documentation inherited
  double distance(
      CollisionGroup* group1,
      CollisionGroup* group2,
      const DistanceOption& option = DistanceOption(false, 0.0, nullptr),
      DistanceResult* result = nullptr) override;

  /// Set primitive shape type
  void setPrimitiveShapeType(PrimitiveShape type);

  /// Get primitive shape type
  PrimitiveShape getPrimitiveShapeType() const;

  /// Set contact point computation method
  void setContactPointComputationMethod(ContactPointComputationMethod method);

  /// Get contact point computation method
  ContactPointComputationMethod getContactPointComputationMethod() const;

protected:
  /// Constructor
  HitCollisionDetector();

  // Documentation inherited
  std::unique_ptr<CollisionObject> createCollisionObject(
      const dynamics::ShapeFrame* shapeFrame) override;

  void refreshCollisionObject(CollisionObject* object) override;

  /// Return HIT CollisionGeometry associated with give Shape. New
  /// CollisionGeometry will be created if it hasn't created yet.
  std::shared_ptr<dart::collision::hit::CollisionGeometry<double>>
  claimHitCollisionGeometry(const dynamics::ConstShapePtr& shape);

protected:
  PrimitiveShape mPrimitiveShapeType;

  ContactPointComputationMethod mContactPointComputationMethod;

private:
  /// This deleter is responsible for deleting HIT CollisionGeometry and
  /// removing it from mShapeMap when it is not shared by any CollisionObjects.
  class HitCollisionGeometryDeleter final
  {
  public:
    HitCollisionGeometryDeleter(
        HitCollisionDetector* cd, const dynamics::ConstShapePtr& shape);

    void operator()(
        dart::collision::hit::CollisionGeometry<double>* geom) const;

  private:
    HitCollisionDetector* mHitCollisionDetector;

    dynamics::ConstShapePtr mShape;
  };

  /// Information for a shape that was generated by this collision detector
  struct ShapeInfo final
  {
    /// A weak reference to the shape
    std::weak_ptr<dart::collision::hit::CollisionGeometry<double>> mShape;

    /// The last version of the shape, as known by this collision detector
    std::size_t mLastKnownVersion;
  };

  /// Create HIT CollisionGeometry with the custom deleter
  /// HitCollisionGeometryDeleter
  std::shared_ptr<dart::collision::hit::CollisionGeometry<double>>
  createHitCollisionGeometry(
      const dynamics::ConstShapePtr& shape,
      HitCollisionDetector::PrimitiveShape type,
      const HitCollisionGeometryDeleter& deleter);

private:
  using ShapeMap = std::map<dynamics::ConstShapePtr, ShapeInfo>;

  ShapeMap mShapeMap;

  static Registrar<HitCollisionDetector> mRegistrar;
};

} // namespace dart::collision

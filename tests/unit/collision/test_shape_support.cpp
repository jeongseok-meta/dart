/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 */

#include <dart/collision/hit/ccd/shape_support.hpp>

#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/CapsuleShape.hpp>
#include <dart/dynamics/SphereShape.hpp>

#include <gtest/gtest.h>

using namespace dart::collision::ccd;
using namespace dart::dynamics;

//==============================================================================
// Test GJK with DART Sphere-Sphere collision
//==============================================================================

TEST(ShapeSupportTest, SphereSphereIntersection)
{
  // Two overlapping spheres
  SphereShape sphere1(1.0);
  SphereShape sphere2(1.0);

  CcdConfig<double> config;
  config.support1 = sphereSupport<double>;
  config.support2 = sphereSupport<double>;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&sphere1, &sphere2, config);
  EXPECT_TRUE(intersects) << "Two spheres at origin should intersect";
}

TEST(ShapeSupportTest, SphereSphereNoIntersection)
{
  // Two separated spheres (shift one by offsetting support function)
  SphereShape sphere1(1.0);
  SphereShape sphere2(1.0);

  // Custom support function that offsets sphere2 by (5, 0, 0)
  auto sphere2SupportOffset = [](const void* obj,
                                   const Eigen::Vector3d& dir,
                                   Eigen::Vector3d& support) {
    const auto* sphere = static_cast<const SphereShape*>(obj);
    double radius = sphere->getRadius();
    double len = dir.norm();
    if (len > 1e-10) {
      support = (dir / len) * radius + Eigen::Vector3d(5.0, 0.0, 0.0);
    } else {
      support = Eigen::Vector3d(5.0 + radius, 0.0, 0.0);
    }
  };

  CcdConfig<double> config;
  config.support1 = sphereSupport<double>;
  config.support2 = sphere2SupportOffset;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&sphere1, &sphere2, config);
  EXPECT_FALSE(intersects) << "Spheres separated by 5 units should not intersect";
}

TEST(ShapeSupportTest, SphereSphereTouching)
{
  // Two touching spheres
  SphereShape sphere1(1.0);
  SphereShape sphere2(1.0);

  // Offset sphere2 by exactly 2.0 units (sum of radii)
  auto sphere2SupportOffset = [](const void* obj,
                                   const Eigen::Vector3d& dir,
                                   Eigen::Vector3d& support) {
    const auto* sphere = static_cast<const SphereShape*>(obj);
    double radius = sphere->getRadius();
    double len = dir.norm();
    if (len > 1e-10) {
      support = (dir / len) * radius + Eigen::Vector3d(2.0, 0.0, 0.0);
    } else {
      support = Eigen::Vector3d(2.0 + radius, 0.0, 0.0);
    }
  };

  CcdConfig<double> config;
  config.support1 = sphereSupport<double>;
  config.support2 = sphere2SupportOffset;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&sphere1, &sphere2, config);
  EXPECT_TRUE(intersects) << "Touching spheres should intersect";
}

//==============================================================================
// Test GJK with DART Box-Box collision
//==============================================================================

TEST(ShapeSupportTest, BoxBoxIntersection)
{
  // Two overlapping boxes
  BoxShape box1(Eigen::Vector3d(2.0, 2.0, 2.0));
  BoxShape box2(Eigen::Vector3d(2.0, 2.0, 2.0));

  CcdConfig<double> config;
  config.support1 = boxSupport<double>;
  config.support2 = boxSupport<double>;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&box1, &box2, config);
  EXPECT_TRUE(intersects) << "Two boxes at origin should intersect";
}

TEST(ShapeSupportTest, BoxBoxNoIntersection)
{
  // Two separated boxes
  BoxShape box1(Eigen::Vector3d(2.0, 2.0, 2.0));
  BoxShape box2(Eigen::Vector3d(2.0, 2.0, 2.0));

  // Offset box2 by (5, 0, 0)
  auto box2SupportOffset = [](const void* obj,
                               const Eigen::Vector3d& dir,
                               Eigen::Vector3d& support) {
    const auto* box = static_cast<const BoxShape*>(obj);
    const Eigen::Vector3d& size = box->getSize();
    support[0] = (dir[0] >= 0.0) ? (size[0] * 0.5) : (-size[0] * 0.5);
    support[1] = (dir[1] >= 0.0) ? (size[1] * 0.5) : (-size[1] * 0.5);
    support[2] = (dir[2] >= 0.0) ? (size[2] * 0.5) : (-size[2] * 0.5);
    support[0] += 5.0; // Offset
  };

  CcdConfig<double> config;
  config.support1 = boxSupport<double>;
  config.support2 = box2SupportOffset;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&box1, &box2, config);
  EXPECT_FALSE(intersects) << "Boxes separated by 5 units should not intersect";
}

//==============================================================================
// Test GJK with DART Sphere-Box collision
//==============================================================================

TEST(ShapeSupportTest, SphereBoxIntersection)
{
  // Sphere and box at origin
  SphereShape sphere(1.0);
  BoxShape box(Eigen::Vector3d(2.0, 2.0, 2.0));

  CcdConfig<double> config;
  config.support1 = sphereSupport<double>;
  config.support2 = boxSupport<double>;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&sphere, &box, config);
  EXPECT_TRUE(intersects) << "Sphere and box at origin should intersect";
}

//==============================================================================
// Test GJK with DART Capsule collision
//==============================================================================

TEST(ShapeSupportTest, CapsuleCapsuleIntersection)
{
  // Two capsules at origin
  CapsuleShape capsule1(0.5, 2.0); // radius, height
  CapsuleShape capsule2(0.5, 2.0);

  CcdConfig<double> config;
  config.support1 = capsuleSupport<double>;
  config.support2 = capsuleSupport<double>;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  bool intersects = gjkIntersect<double>(&capsule1, &capsule2, config);
  EXPECT_TRUE(intersects) << "Two capsules at origin should intersect";
}

//==============================================================================
// Test MPR penetration with DART shapes
//==============================================================================

TEST(ShapeSupportTest, SphereSphereMPRPenetration)
{
  // Two overlapping spheres
  SphereShape sphere1(1.0);
  SphereShape sphere2(1.0);

  // Offset sphere2 by (0.5, 0, 0) - significant overlap
  auto sphere2SupportOffset = [](const void* obj,
                                   const Eigen::Vector3d& dir,
                                   Eigen::Vector3d& support) {
    const auto* sphere = static_cast<const SphereShape*>(obj);
    double radius = sphere->getRadius();
    double len = dir.norm();
    if (len > 1e-10) {
      support = (dir / len) * radius + Eigen::Vector3d(0.5, 0.0, 0.0);
    } else {
      support = Eigen::Vector3d(0.5 + radius, 0.0, 0.0);
    }
  };

  CcdConfig<double> config;
  config.support1 = sphereSupport<double>;
  config.support2 = sphere2SupportOffset;
  config.center1 = shapeCenter<double>;
  config.center2 = shapeCenter<double>;

  double depth;
  Eigen::Vector3d direction, position;
  bool success = mprPenetration<double>(
      &sphere1, &sphere2, config, &depth, &direction, &position);

  EXPECT_TRUE(success) << "MPR should detect penetration";
  EXPECT_GT(depth, 0.0) << "Penetration depth should be positive";
  EXPECT_LT(depth, 2.0) << "Penetration depth should be less than combined radii";
}

//==============================================================================
// Test configureCcdForShape helper
//==============================================================================

TEST(ShapeSupportTest, ConfigureCcdForSphere)
{
  SphereShape sphere(1.0);

  CcdConfig<double> config;
  configureCcdForShape(&sphere, config);

  EXPECT_TRUE(config.support1 != nullptr);
  EXPECT_TRUE(config.support2 != nullptr);
  EXPECT_TRUE(config.center1 != nullptr);
  EXPECT_TRUE(config.center2 != nullptr);
}

TEST(ShapeSupportTest, ConfigureCcdForBox)
{
  BoxShape box(Eigen::Vector3d(2.0, 2.0, 2.0));

  CcdConfig<double> config;
  configureCcdForShape(&box, config);

  EXPECT_TRUE(config.support1 != nullptr);
  EXPECT_TRUE(config.support2 != nullptr);
}

TEST(ShapeSupportTest, ConfigureCcdForCapsule)
{
  CapsuleShape capsule(0.5, 2.0);

  CcdConfig<double> config;
  configureCcdForShape(&capsule, config);

  EXPECT_TRUE(config.support1 != nullptr);
  EXPECT_TRUE(config.support2 != nullptr);
}

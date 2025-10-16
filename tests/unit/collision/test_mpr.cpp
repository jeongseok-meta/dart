/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Modernized C++ implementation of libccd (Continuous Collision Detection)
 * Based on libccd by Daniel Fiser <danfis@danfis.cz>
 */

#include "dart/collision/hit/ccd/ccd.hpp"

#include <gtest/gtest.h>

#include <ccd/ccd.h>

#include <cstdlib>
#include <cstring>

using namespace dart::collision::ccd;

//==============================================================================
/// Sphere collision object
struct Sphere
{
  Eigen::Vector3d pos;
  double radius;
};

//==============================================================================
/// Support function for sphere (DART C++ version)
void sphereSupport(
    const void* obj, const Eigen::Vector3d& dir, Eigen::Vector3d& support)
{
  const auto* sphere = static_cast<const Sphere*>(obj);
  Eigen::Vector3d normalized = dir;
  normalized.normalize();
  support = sphere->pos + normalized * sphere->radius;
}

/// Center function for sphere (DART C++ version)
void sphereCenter(const void* obj, Eigen::Vector3d& center)
{
  const auto* sphere = static_cast<const Sphere*>(obj);
  center = sphere->pos;
}

//==============================================================================
/// Support function for sphere (upstream libccd)
void ccdSphereSupport(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* v)
{
  const auto* sphere = static_cast<const Sphere*>(obj);
  ccd_vec3_t dirNorm;
  ccdVec3Copy(&dirNorm, dir);
  ccdVec3Normalize(&dirNorm);

  v->v[0] = sphere->pos.x() + dirNorm.v[0] * sphere->radius;
  v->v[1] = sphere->pos.y() + dirNorm.v[1] * sphere->radius;
  v->v[2] = sphere->pos.z() + dirNorm.v[2] * sphere->radius;
}

/// Center function for sphere (upstream libccd)
void ccdSphereCenter(const void* obj, ccd_vec3_t* center)
{
  const auto* sphere = static_cast<const Sphere*>(obj);
  center->v[0] = sphere->pos.x();
  center->v[1] = sphere->pos.y();
  center->v[2] = sphere->pos.z();
}

//==============================================================================
/// Test fixture for MPR tests
class MprTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    // Configure DART C++ version
    config.support1 = sphereSupport;
    config.support2 = sphereSupport;
    config.center1 = sphereCenter;
    config.center2 = sphereCenter;
    config.maxIterations = 100;

    // Configure upstream libccd
    std::memset(&ccd, 0, sizeof(ccd));
    ccd.support1 = ccdSphereSupport;
    ccd.support2 = ccdSphereSupport;
    ccd.center1 = ccdSphereCenter;
    ccd.center2 = ccdSphereCenter;
    ccd.max_iterations = 100;
    ccd.mpr_tolerance = CCD_REAL(1e-4);
  }

  CcdConfig<double> config;
  ccd_t ccd;
};

//==============================================================================
TEST_F(MprTest, SphereSphereIntersection)
{
  // Test case 1: Overlapping spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.5, 0.0, 0.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 2: Non-overlapping spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{3.0, 0.0, 0.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_FALSE(intersectCpp);
    EXPECT_FALSE(intersectC);
  }

  // Test case 3: Touching spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{2.0, 0.0, 0.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_EQ(intersectCpp, static_cast<bool>(intersectC));
  }

  // Test case 4: Spheres at origin
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.0, 0.0, 0.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 5: Different Y positions
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.0, 1.5, 0.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 6: Different Z positions
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.0, 0.0, 1.8}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 7: Far apart spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{10.0, 10.0, 10.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_FALSE(intersectCpp);
    EXPECT_FALSE(intersectC);
  }

  // Test case 8: Small spheres overlapping
  {
    Sphere s1{{0.0, 0.0, 0.0}, 0.1};
    Sphere s2{{0.1, 0.0, 0.0}, 0.1};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 9: Large spheres overlapping
  {
    Sphere s1{{0.0, 0.0, 0.0}, 10.0};
    Sphere s2{{5.0, 0.0, 0.0}, 10.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }

  // Test case 10: Diagonal positioning
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{1.0, 1.0, 1.0}, 1.0};

    // DART C++ version
    bool intersectCpp = mprIntersect(&s1, &s2, config);

    // Upstream libccd
    int intersectC = ccdMPRIntersect(&s1, &s2, &ccd);

    EXPECT_TRUE(intersectCpp);
    EXPECT_TRUE(intersectC);
  }
}

//==============================================================================
TEST_F(MprTest, SphereSphereePenetration)
{
  // Test case 1: Overlapping spheres - check penetration depth
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.5, 0.0, 0.0}, 1.0};

    // DART C++ version
    double depthCpp;
    Eigen::Vector3d directionCpp, positionCpp;
    bool successCpp = mprPenetration(
        &s1, &s2, config, &depthCpp, &directionCpp, &positionCpp);

    // Upstream libccd
    ccd_real_t depthC;
    ccd_vec3_t directionC, positionC;
    int successC
        = ccdMPRPenetration(&s1, &s2, &ccd, &depthC, &directionC, &positionC);

    EXPECT_TRUE(successCpp);
    EXPECT_EQ(0, successC);

    // Expected penetration depth: 1.5 (2.0 - 0.5)
    EXPECT_NEAR(depthCpp, 1.5, 1e-3);
    EXPECT_NEAR(depthC, 1.5, 1e-3);
  }

  // Test case 2: Non-overlapping spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{3.0, 0.0, 0.0}, 1.0};

    // DART C++ version
    double depthCpp;
    Eigen::Vector3d directionCpp, positionCpp;
    bool successCpp = mprPenetration(
        &s1, &s2, config, &depthCpp, &directionCpp, &positionCpp);

    // Upstream libccd
    ccd_real_t depthC;
    ccd_vec3_t directionC, positionC;
    int successC
        = ccdMPRPenetration(&s1, &s2, &ccd, &depthC, &directionC, &positionC);

    EXPECT_FALSE(successCpp);
    EXPECT_NE(0, successC); // libccd returns -1 for no collision
  }

  // Test case 3: Deeply overlapping spheres
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.0, 0.0, 0.0}, 1.0};

    // DART C++ version
    double depthCpp;
    Eigen::Vector3d directionCpp, positionCpp;
    bool successCpp = mprPenetration(
        &s1, &s2, config, &depthCpp, &directionCpp, &positionCpp);

    // Upstream libccd
    ccd_real_t depthC;
    ccd_vec3_t directionC, positionC;
    int successC
        = ccdMPRPenetration(&s1, &s2, &ccd, &depthC, &directionC, &positionC);

    EXPECT_TRUE(successCpp);
    EXPECT_EQ(0, successC);

    // Expected penetration depth: 2.0 (full overlap)
    EXPECT_NEAR(depthCpp, 2.0, 1e-3);
    EXPECT_NEAR(depthC, 2.0, 1e-3);
  }

  // Test case 4: Slight overlap
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{1.9, 0.0, 0.0}, 1.0};

    // DART C++ version
    double depthCpp;
    Eigen::Vector3d directionCpp, positionCpp;
    bool successCpp = mprPenetration(
        &s1, &s2, config, &depthCpp, &directionCpp, &positionCpp);

    // Upstream libccd
    ccd_real_t depthC;
    ccd_vec3_t directionC, positionC;
    int successC
        = ccdMPRPenetration(&s1, &s2, &ccd, &depthC, &directionC, &positionC);

    EXPECT_TRUE(successCpp);
    EXPECT_EQ(0, successC);

    // Expected penetration depth: 0.1 (2.0 - 1.9)
    EXPECT_NEAR(depthCpp, 0.1, 1e-3);
    EXPECT_NEAR(depthC, 0.1, 1e-3);
  }

  // Test case 5: Y-axis overlap
  {
    Sphere s1{{0.0, 0.0, 0.0}, 1.0};
    Sphere s2{{0.0, 1.5, 0.0}, 1.0};

    // DART C++ version
    double depthCpp;
    Eigen::Vector3d directionCpp, positionCpp;
    bool successCpp = mprPenetration(
        &s1, &s2, config, &depthCpp, &directionCpp, &positionCpp);

    // Upstream libccd
    ccd_real_t depthC;
    ccd_vec3_t directionC, positionC;
    int successC
        = ccdMPRPenetration(&s1, &s2, &ccd, &depthC, &directionC, &positionC);

    EXPECT_TRUE(successCpp);
    EXPECT_EQ(0, successC);

    // Expected penetration depth: 0.5 (2.0 - 1.5)
    EXPECT_NEAR(depthCpp, 0.5, 1e-3);
    EXPECT_NEAR(depthC, 0.5, 1e-3);
  }
}

//==============================================================================
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

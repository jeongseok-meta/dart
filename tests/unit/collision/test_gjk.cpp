// Test for modernized GJK implementation
// Compares C++ template implementation against upstream libccd

#include <Eigen/Core>
#include <gtest/gtest.h>

// Include modernized C++ template implementation
#include "dart/collision/hit/ccd/ccd.hpp"

// Include upstream libccd for comparison (required transitive dependency from
// fcl)
extern "C" {
#include <ccd/ccd.h>
}

using namespace dart::collision::ccd;

// Simple sphere shape for testing
struct Sphere
{
  Eigen::Vector3d center;
  double radius;

  Sphere(const Eigen::Vector3d& c, double r) : center(c), radius(r) {}
};

// Support function for C++ implementation
void sphereSupportCpp(
    const void* obj, const Eigen::Vector3d& dir, Eigen::Vector3d& out)
{
  const Sphere* sphere = static_cast<const Sphere*>(obj);
  Eigen::Vector3d normalized = dir.normalized();
  out = sphere->center + normalized * sphere->radius;
}

// Center function for C++ implementation
void sphereCenterCpp(const void* obj, Eigen::Vector3d& out)
{
  const Sphere* sphere = static_cast<const Sphere*>(obj);
  out = sphere->center;
}

// Support function for upstream C implementation
void sphereSupportUpstream(
    const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* out)
{
  const Sphere* sphere = static_cast<const Sphere*>(obj);
  ccd_real_t len = CCD_SQRT(ccdVec3Len2(dir));
  if (len > 1e-10) {
    ccdVec3Copy(out, dir);
    ccdVec3Scale(out, sphere->radius / len);
    out->v[0] += sphere->center.x();
    out->v[1] += sphere->center.y();
    out->v[2] += sphere->center.z();
  } else {
    out->v[0] = sphere->center.x() + sphere->radius;
    out->v[1] = sphere->center.y();
    out->v[2] = sphere->center.z();
  }
}

// Center function for upstream C implementation
void sphereCenterUpstream(const void* obj, ccd_vec3_t* out)
{
  const Sphere* sphere = static_cast<const Sphere*>(obj);
  out->v[0] = sphere->center.x();
  out->v[1] = sphere->center.y();
  out->v[2] = sphere->center.z();
}

// Test fixture for GJK tests
class GJKTest : public ::testing::Test
{
protected:
  void testIntersection(
      const Sphere& s1,
      const Sphere& s2,
      bool expectedResult,
      const char* testName)
  {
    // Test C++ template implementation
    CcdConfig<double> configCpp;
    configCpp.support1 = sphereSupportCpp;
    configCpp.support2 = sphereSupportCpp;
    configCpp.center1 = sphereCenterCpp;
    configCpp.center2 = sphereCenterCpp;
    configCpp.maxIterations = 100;

    bool resultCpp = gjkIntersect(&s1, &s2, configCpp);

    EXPECT_EQ(resultCpp, expectedResult)
        << testName << ": C++ result doesn't match expected value";

    // Test upstream C implementation for comparison
    ccd_t configUpstream;
    CCD_INIT(&configUpstream);
    configUpstream.support1 = sphereSupportUpstream;
    configUpstream.support2 = sphereSupportUpstream;
    configUpstream.center1 = sphereCenterUpstream;
    configUpstream.center2 = sphereCenterUpstream;
    configUpstream.max_iterations = 100;

    int upstreamResult = ccdGJKIntersect(&s1, &s2, &configUpstream);
    bool resultUpstream = (upstreamResult == 1);

    EXPECT_EQ(resultCpp, resultUpstream)
        << testName << ": C++ and upstream implementations disagree";
  }
};

TEST_F(GJKTest, SphereSphereIntersecting)
{
  // Two spheres that are clearly intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(1.5, 0, 0), 1.0);

  testIntersection(s1, s2, true, "SphereSphereIntersecting");
}

TEST_F(GJKTest, SphereSphereNotIntersecting)
{
  // Two spheres that are NOT intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(3.0, 0, 0), 1.0);

  testIntersection(s1, s2, false, "SphereSphereNotIntersecting");
}

TEST_F(GJKTest, SphereSphereTouching)
{
  // Two spheres that are just touching
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(2.0, 0, 0), 1.0);

  // Just touching is considered intersecting in GJK
  testIntersection(s1, s2, true, "SphereSphereTouching");
}

TEST_F(GJKTest, SphereSphereIdentical)
{
  // Two identical spheres (complete overlap)
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(0, 0, 0), 1.0);

  testIntersection(s1, s2, true, "SphereSphereIdentical");
}

TEST_F(GJKTest, SphereSphereOffsetY)
{
  // Spheres offset on Y axis, intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(0, 1.2, 0), 1.0);

  testIntersection(s1, s2, true, "SphereSphereOffsetY");
}

TEST_F(GJKTest, SphereSphereOffsetZ)
{
  // Spheres offset on Z axis, not intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.0);
  Sphere s2(Eigen::Vector3d(0, 0, 2.5), 1.0);

  testIntersection(s1, s2, false, "SphereSphereOffsetZ");
}

TEST_F(GJKTest, SphereSphere3DOffset)
{
  // Spheres offset in 3D, intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 1.5);
  Sphere s2(Eigen::Vector3d(1, 1, 1), 1.5);

  // Distance between centers: sqrt(1 + 1 + 1) = sqrt(3) ≈ 1.732
  // Sum of radii: 3.0
  // Should intersect
  testIntersection(s1, s2, true, "SphereSphere3DOffset");
}

TEST_F(GJKTest, SphereSphereSmall)
{
  // Very small spheres, intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 0.1);
  Sphere s2(Eigen::Vector3d(0.15, 0, 0), 0.1);

  testIntersection(s1, s2, true, "SphereSphereSmall");
}

TEST_F(GJKTest, SphereSphereSmallNotIntersecting)
{
  // Very small spheres, not intersecting
  Sphere s1(Eigen::Vector3d(0, 0, 0), 0.1);
  Sphere s2(Eigen::Vector3d(0.25, 0, 0), 0.1);

  testIntersection(s1, s2, false, "SphereSphereSmallNotIntersecting");
}

TEST_F(GJKTest, SphereSphereNegativeOffset)
{
  // Spheres with negative coordinates
  Sphere s1(Eigen::Vector3d(-5, -5, -5), 1.0);
  Sphere s2(Eigen::Vector3d(-5.8, -5, -5), 1.0);

  testIntersection(s1, s2, true, "SphereSphereNegativeOffset");
}

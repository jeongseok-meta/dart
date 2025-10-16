/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 */

#include <dart/collision/hit/ccd/ccd.hpp>

#include <gtest/gtest.h>

// Include upstream libccd for comparison (required transitive dependency from fcl)
extern "C" {
#include <ccd/ccd.h>
}

using namespace dart::collision::ccd;

//==============================================================================
// Typed test fixture
//==============================================================================

template <typename T>
class CcdTypedTest : public ::testing::Test
{
public:
  using Scalar = T;
  using Vector3 = Eigen::Matrix<T, 3, 1>;
};

using ScalarTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(CcdTypedTest, ScalarTypes);

//==============================================================================
// Test basic structures
//==============================================================================

TYPED_TEST(CcdTypedTest, SupportPoint)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  SupportPoint<Scalar> sp;

  Vector3 v1(Scalar(1.0), Scalar(2.0), Scalar(3.0));
  Vector3 v2(Scalar(0.5), Scalar(1.0), Scalar(1.5));
  Vector3 v = v1 - v2;

  sp = SupportPoint<Scalar>(v, v1, v2);

  EXPECT_EQ(sp.v.x(), Scalar(0.5));
  EXPECT_EQ(sp.v.y(), Scalar(1.0));
  EXPECT_EQ(sp.v.z(), Scalar(1.5));
  EXPECT_EQ(sp.v1.x(), Scalar(1.0));
  EXPECT_EQ(sp.v2.x(), Scalar(0.5));
}

//==============================================================================
// Test Simplex
//==============================================================================

TYPED_TEST(CcdTypedTest, Simplex)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  Simplex<Scalar> simplex;

  EXPECT_EQ(simplex.size(), 0);

  SupportPoint<Scalar> p1(Vector3(1, 0, 0), Vector3(1, 0, 0), Vector3::Zero());
  simplex.add(p1);
  EXPECT_EQ(simplex.size(), 1);
  EXPECT_EQ(simplex[0].v.x(), Scalar(1.0));

  SupportPoint<Scalar> p2(Vector3(0, 1, 0), Vector3(0, 1, 0), Vector3::Zero());
  simplex.add(p2);
  EXPECT_EQ(simplex.size(), 2);
  EXPECT_EQ(simplex[1].v.y(), Scalar(1.0));

  simplex.swap(0, 1);
  EXPECT_EQ(simplex[0].v.y(), Scalar(1.0));
  EXPECT_EQ(simplex[1].v.x(), Scalar(1.0));

  simplex.setSize(1);
  EXPECT_EQ(simplex.size(), 1);
}

//==============================================================================
// Test utility functions
//==============================================================================

TYPED_TEST(CcdTypedTest, IsZero)
{
  using Scalar = typename TestFixture::Scalar;

  EXPECT_TRUE(isZero<Scalar>(Scalar(0.0)));
  EXPECT_TRUE(isZero<Scalar>(Scalar(1e-11)));
  EXPECT_FALSE(isZero<Scalar>(Scalar(1e-9)));
  EXPECT_FALSE(isZero<Scalar>(Scalar(1.0)));
}

TYPED_TEST(CcdTypedTest, IsEqual)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  EXPECT_TRUE(isEqual<Scalar>(Scalar(1.0), Scalar(1.0)));
  EXPECT_TRUE(isEqual<Scalar>(Scalar(1.0), Scalar(1.0) + Scalar(1e-11)));
  EXPECT_FALSE(isEqual<Scalar>(Scalar(1.0), Scalar(1.1)));

  Vector3 v1(Scalar(1.0), Scalar(2.0), Scalar(3.0));
  Vector3 v2(Scalar(1.0), Scalar(2.0), Scalar(3.0));
  Vector3 v3(Scalar(1.1), Scalar(2.0), Scalar(3.0));

  EXPECT_TRUE(isEqual(v1, v2));
  EXPECT_FALSE(isEqual(v1, v3));
}

//==============================================================================
// Test default functions
//==============================================================================

TYPED_TEST(CcdTypedTest, DefaultFirstDir)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  Vector3 dir;
  defaultFirstDir<Scalar>(nullptr, nullptr, dir);

  EXPECT_EQ(dir.x(), Scalar(1.0));
  EXPECT_EQ(dir.y(), Scalar(0.0));
  EXPECT_EQ(dir.z(), Scalar(0.0));
}

TYPED_TEST(CcdTypedTest, GjkIntersectStub)
{
  using Scalar = typename TestFixture::Scalar;

  CcdConfig<Scalar> config;
  EXPECT_FALSE(gjkIntersect<Scalar>(nullptr, nullptr, config));
}

//==============================================================================
// Test GJK with simple shapes
//==============================================================================

// Sphere shape for testing
template <typename S>
struct Sphere
{
  Eigen::Matrix<S, 3, 1> center;
  S radius;
};

// Support function for C++ implementation
template <typename S>
void sphereSupport(
    const void* obj,
    const Eigen::Matrix<S, 3, 1>& dir,
    Eigen::Matrix<S, 3, 1>& support)
{
  const auto* sphere = static_cast<const Sphere<S>*>(obj);
  S len = dir.norm();
  if (len > S(1e-10)) {
    support = sphere->center + (dir / len) * sphere->radius;
  } else {
    support = sphere->center;
  }
}

// Support function for upstream C implementation
void sphereSupportUpstream(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* out)
{
  const auto* sphere = static_cast<const Sphere<double>*>(obj);
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
  const auto* sphere = static_cast<const Sphere<double>*>(obj);
  out->v[0] = sphere->center.x();
  out->v[1] = sphere->center.y();
  out->v[2] = sphere->center.z();
}

TYPED_TEST(CcdTypedTest, GjkSphereSphereIntersect)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Two overlapping spheres
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(1.0)};
  Sphere<Scalar> sphere2{Vector3(Scalar(1.5), 0, 0), Scalar(1.0)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_TRUE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

TYPED_TEST(CcdTypedTest, GjkSphereSphereNoIntersect)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Two separated spheres
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(1.0)};
  Sphere<Scalar> sphere2{Vector3(Scalar(3.0), 0, 0), Scalar(1.0)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_FALSE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

TYPED_TEST(CcdTypedTest, GjkSphereSphereTouch)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Two touching spheres (barely intersecting)
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(1.0)};
  Sphere<Scalar> sphere2{Vector3(Scalar(2.0), 0, 0), Scalar(1.0)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_TRUE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

TYPED_TEST(CcdTypedTest, GjkSphereSphereContained)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Small sphere inside larger sphere
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(2.0)};
  Sphere<Scalar> sphere2{Vector3(Scalar(0.5), 0, 0), Scalar(0.5)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_TRUE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

TYPED_TEST(CcdTypedTest, GjkSphereSphereDifferentAxes)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Test on different axes
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(1.0)};
  Sphere<Scalar> sphere2{Vector3(0, Scalar(1.5), 0), Scalar(1.0)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_TRUE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

TYPED_TEST(CcdTypedTest, GjkSphereSphere3DIntersect)
{
  using Scalar = typename TestFixture::Scalar;
  using Vector3 = typename TestFixture::Vector3;

  // Test in 3D
  Sphere<Scalar> sphere1{Vector3(0, 0, 0), Scalar(1.0)};
  Sphere<Scalar> sphere2{
      Vector3(Scalar(0.5), Scalar(0.5), Scalar(0.5)), Scalar(1.0)};

  CcdConfig<Scalar> config;
  config.support1 = sphereSupport<Scalar>;
  config.support2 = sphereSupport<Scalar>;

  EXPECT_TRUE(gjkIntersect<Scalar>(&sphere1, &sphere2, config));
}

//==============================================================================
// Compare C++ implementation against upstream libccd
//==============================================================================

TEST(CcdTest, CompareAgainstUpstreamIntersect)
{
  // Test intersecting spheres
  Sphere<double> sphere1{Eigen::Vector3d(0, 0, 0), 1.0};
  Sphere<double> sphere2{Eigen::Vector3d(1.5, 0, 0), 1.0};

  // C++ implementation
  CcdConfig<double> configCpp;
  configCpp.support1 = sphereSupport<double>;
  configCpp.support2 = sphereSupport<double>;
  bool resultCpp = gjkIntersect<double>(&sphere1, &sphere2, configCpp);

  // Upstream C implementation
  ccd_t configUpstream;
  CCD_INIT(&configUpstream);
  configUpstream.support1 = sphereSupportUpstream;
  configUpstream.support2 = sphereSupportUpstream;
  configUpstream.center1 = sphereCenterUpstream;
  configUpstream.center2 = sphereCenterUpstream;
  int upstreamResult = ccdGJKIntersect(&sphere1, &sphere2, &configUpstream);
  bool resultUpstream = (upstreamResult == 1);

  EXPECT_EQ(resultCpp, resultUpstream)
      << "C++ and upstream implementations disagree on intersecting spheres";
  EXPECT_TRUE(resultCpp) << "Both should detect intersection";
}

TEST(CcdTest, CompareAgainstUpstreamNoIntersect)
{
  // Test separated spheres
  Sphere<double> sphere1{Eigen::Vector3d(0, 0, 0), 1.0};
  Sphere<double> sphere2{Eigen::Vector3d(3.0, 0, 0), 1.0};

  // C++ implementation
  CcdConfig<double> configCpp;
  configCpp.support1 = sphereSupport<double>;
  configCpp.support2 = sphereSupport<double>;
  bool resultCpp = gjkIntersect<double>(&sphere1, &sphere2, configCpp);

  // Upstream C implementation
  ccd_t configUpstream;
  CCD_INIT(&configUpstream);
  configUpstream.support1 = sphereSupportUpstream;
  configUpstream.support2 = sphereSupportUpstream;
  configUpstream.center1 = sphereCenterUpstream;
  configUpstream.center2 = sphereCenterUpstream;
  int upstreamResult = ccdGJKIntersect(&sphere1, &sphere2, &configUpstream);
  bool resultUpstream = (upstreamResult == 1);

  EXPECT_EQ(resultCpp, resultUpstream)
      << "C++ and upstream implementations disagree on separated spheres";
  EXPECT_FALSE(resultCpp) << "Both should detect no intersection";
}

TEST(CcdTest, CompareAgainstUpstreamTouching)
{
  // Test touching spheres
  Sphere<double> sphere1{Eigen::Vector3d(0, 0, 0), 1.0};
  Sphere<double> sphere2{Eigen::Vector3d(2.0, 0, 0), 1.0};

  // C++ implementation
  CcdConfig<double> configCpp;
  configCpp.support1 = sphereSupport<double>;
  configCpp.support2 = sphereSupport<double>;
  bool resultCpp = gjkIntersect<double>(&sphere1, &sphere2, configCpp);

  // Upstream C implementation
  ccd_t configUpstream;
  CCD_INIT(&configUpstream);
  configUpstream.support1 = sphereSupportUpstream;
  configUpstream.support2 = sphereSupportUpstream;
  configUpstream.center1 = sphereCenterUpstream;
  configUpstream.center2 = sphereCenterUpstream;
  int upstreamResult = ccdGJKIntersect(&sphere1, &sphere2, &configUpstream);
  bool resultUpstream = (upstreamResult == 1);

  EXPECT_EQ(resultCpp, resultUpstream)
      << "C++ and upstream implementations disagree on touching spheres";
}

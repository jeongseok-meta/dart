/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Modernized C++ implementation of libccd (Continuous Collision Detection)
 * Based on libccd by Daniel Fiser <danfis@danfis.cz>
 */

#pragma once

#include "simplex.hpp"
#include "support.hpp"

#include <Eigen/Geometry> // Needed for cross() method

#include <functional>
#include <limits>

namespace dart::collision::ccd {

//==============================================================================
/// Configuration for CCD algorithm
template <typename S>
struct CcdConfig
{
  using Vector3 = Eigen::Vector3<S>;
  using SupportFunction = std::function<void(
      const void* obj, const Vector3& dir, Vector3& support)>;
  using FirstDirFunction
      = std::function<void(const void* obj1, const void* obj2, Vector3& dir)>;
  using CenterFunction = std::function<void(const void* obj, Vector3& center)>;

  SupportFunction support1;    ///< Support function for first object
  SupportFunction support2;    ///< Support function for second object
  FirstDirFunction firstDir;   ///< Initial direction function
  CenterFunction center1;      ///< Center function for first object
  CenterFunction center2;      ///< Center function for second object
  unsigned long maxIterations; ///< Maximum number of iterations
  S epaTolerance;              ///< EPA (Expanding Polytope Algorithm) tolerance
  S mprTolerance;              ///< MPR (Minkowski Portal Refinement) tolerance
  S distTolerance;             ///< Distance tolerance

  CcdConfig()
    : maxIterations(std::numeric_limits<unsigned long>::max()),
      epaTolerance(S(1e-4)),
      mprTolerance(S(1e-4)),
      distTolerance(S(1e-6))
  {
  }
};

//==============================================================================
/// Utility functions
//==============================================================================

/// Check if two scalar values are approximately equal
template <
    typename S,
    typename = typename std::enable_if<std::is_arithmetic<S>::value>::type>
inline bool isEqual(S a, S b)
{
  constexpr S tolerance = S(1e-10);
  const S diff = std::abs(a - b);
  if (diff < tolerance)
    return true;

  const S absA = std::abs(a);
  const S absB = std::abs(b);
  return diff < tolerance * std::max(absA, absB);
}

/// Check if two vectors are approximately equal
template <typename Derived1, typename Derived2>
inline bool isEqual(
    const Eigen::MatrixBase<Derived1>& a,
    const Eigen::MatrixBase<Derived2>& b)
{
  using Scalar = typename Derived1::Scalar;
  return isEqual<Scalar>(a.x(), b.x()) && isEqual<Scalar>(a.y(), b.y())
         && isEqual<Scalar>(a.z(), b.z());
}
template <typename S>
inline bool isZero(S value)
{
  constexpr S epsilon = S(1e-10);
  return std::abs(value) < epsilon;
}

/// Return sign of value (-1, 0, or 1)
template <typename S>
inline int sign(S value)
{
  if (isZero(value))
    return 0;
  return (value < S(0)) ? -1 : 1;
}

/// Triple cross product: d = a × b × c
template <typename S>
inline void tripleCross(
    const Eigen::Vector3<S>& a,
    const Eigen::Vector3<S>& b,
    const Eigen::Vector3<S>& c,
    Eigen::Vector3<S>& d)
{
  Eigen::Vector3<S> e = a.cross(b);
  d = e.cross(c);
}

//==============================================================================
/// Default first direction function
template <typename S>
void defaultFirstDir(
    const void* /*obj1*/, const void* /*obj2*/, Eigen::Vector3<S>& dir)
{
  dir.setZero();
  dir[0] = S(1.0);
}

//==============================================================================
/// Compute support point in Minkowski difference
template <typename S>
inline void computeSupport(
    const void* obj1,
    const void* obj2,
    const Eigen::Vector3<S>& dir,
    const CcdConfig<S>& config,
    SupportPoint<S>& support)
{
  using Vector3 = Eigen::Vector3<S>;
  Vector3 v1, v2;

  config.support1(obj1, dir, v1);
  config.support2(obj2, -dir, v2);

  support.v1 = v1;
  support.v2 = v2;
  support.v = v1 - v2;
}
template <typename S>
int doSimplex2(Simplex<S>& simplex, Eigen::Vector3<S>& dir)
{
  using Vector3 = Eigen::Vector3<S>;

  const auto& A = simplex[simplex.size() - 1];
  const auto& B = simplex[0];

  Vector3 AB = B.v - A.v;
  Vector3 AO = -A.v;
  S dot = AB.dot(AO);

  Vector3 tmp = AB.cross(AO);
  if (isZero(tmp.squaredNorm()) && dot > S(0)) {
    return 1;
  }

  if (isZero(dot) || dot < S(0)) {
    simplex[0] = A;
    simplex.setSize(1);
    dir = AO;
  } else {
    tripleCross(AB, AO, AB, dir);
  }

  return 0;
}

/// Process 3-point simplex (triangle)
template <typename S>
int doSimplex3(Simplex<S>& simplex, Eigen::Vector3<S>& dir)
{
  using Vector3 = Eigen::Vector3<S>;

  const auto& A = simplex[simplex.size() - 1];
  const auto& B = simplex[1];
  const auto& C = simplex[0];

  if (isEqual(A.v, B.v) || isEqual(A.v, C.v)) {
    return -1;
  }

  Vector3 AO = -A.v;
  Vector3 AB = B.v - A.v;
  Vector3 AC = C.v - A.v;
  Vector3 ABC = AB.cross(AC);

  Vector3 ABC_AC = ABC.cross(AC);
  S dot = ABC_AC.dot(AO);
  if (isZero(dot) || dot > S(0)) {
    dot = AC.dot(AO);
    if (isZero(dot) || dot > S(0)) {
      simplex[1] = A;
      simplex.setSize(2);
      tripleCross(AC, AO, AC, dir);
      return 0;
    } else {
      dot = AB.dot(AO);
      if (isZero(dot) || dot > S(0)) {
        simplex[0] = B;
        simplex[1] = A;
        simplex.setSize(2);
        tripleCross(AB, AO, AB, dir);
        return 0;
      } else {
        simplex[0] = A;
        simplex.setSize(1);
        dir = AO;
        return 0;
      }
    }
  } else {
    Vector3 AB_ABC = AB.cross(ABC);
    dot = AB_ABC.dot(AO);
    if (isZero(dot) || dot > S(0)) {
      dot = AB.dot(AO);
      if (isZero(dot) || dot > S(0)) {
        simplex[0] = B;
        simplex[1] = A;
        simplex.setSize(2);
        tripleCross(AB, AO, AB, dir);
        return 0;
      } else {
        simplex[0] = A;
        simplex.setSize(1);
        dir = AO;
        return 0;
      }
    } else {
      dot = ABC.dot(AO);
      if (isZero(dot) || dot > S(0)) {
        dir = ABC;
        return 0;
      } else {
        SupportPoint<S> Ctmp = C;
        simplex[0] = B;
        simplex[1] = Ctmp;
        dir = -ABC;
        return 0;
      }
    }
  }
}

/// Process 4-point simplex (tetrahedron)
template <typename S>
int doSimplex4(Simplex<S>& simplex, Eigen::Vector3<S>& dir)
{
  using Vector3 = Eigen::Vector3<S>;

  const auto& A = simplex[3];
  const auto& B = simplex[2];
  const auto& C = simplex[1];
  const auto& D = simplex[0];

  Vector3 AO = -A.v;
  Vector3 AB = B.v - A.v;
  Vector3 AC = C.v - A.v;
  Vector3 AD = D.v - A.v;

  Vector3 ABC = AB.cross(AC);
  Vector3 ACD = AC.cross(AD);
  Vector3 ADB = AD.cross(AB);

  int B_on_ACD = sign(ACD.dot(AB));
  int C_on_ADB = sign(ADB.dot(AC));
  int D_on_ABC = sign(ABC.dot(AD));

  bool AB_O = (sign(ACD.dot(AO)) == B_on_ACD);
  bool AC_O = (sign(ADB.dot(AO)) == C_on_ADB);
  bool AD_O = (sign(ABC.dot(AO)) == D_on_ABC);

  if (AB_O && AC_O && AD_O) {
    return 1;
  }

  if (!AB_O) {
    simplex[2] = A;
    simplex.setSize(3);
  } else if (!AC_O) {
    simplex[1] = D;
    simplex[0] = B;
    simplex[2] = A;
    simplex.setSize(3);
  } else {
    simplex[0] = C;
    simplex[1] = B;
    simplex[2] = A;
    simplex.setSize(3);
  }

  return doSimplex3(simplex, dir);
}

/// Main simplex processor
template <typename S>
int doSimplex(Simplex<S>& simplex, Eigen::Vector3<S>& dir)
{
  switch (simplex.size()) {
    case 2:
      return doSimplex2(simplex, dir);
    case 3:
      return doSimplex3(simplex, dir);
    case 4:
      return doSimplex4(simplex, dir);
    default:
      return -1;
  }
}

//==============================================================================
/// GJK-based intersection test
template <typename S>
bool gjkIntersect(
    const void* obj1, const void* obj2, const CcdConfig<S>& config)
{
  using Vector3 = Eigen::Vector3<S>;

  Simplex<S> simplex;
  Vector3 dir;
  SupportPoint<S> last;

  if (config.firstDir) {
    config.firstDir(obj1, obj2, dir);
  } else {
    defaultFirstDir(obj1, obj2, dir);
  }

  computeSupport(obj1, obj2, dir, config, last);
  simplex.add(last);

  dir = -last.v;

  for (unsigned long iter = 0; iter < config.maxIterations; ++iter) {
    computeSupport(obj1, obj2, dir, config, last);

    if (last.v.dot(dir) < S(0)) {
      return false;
    }

    simplex.add(last);

    int result = doSimplex(simplex, dir);
    if (result == 1) {
      return true;
    } else if (result == -1) {
      return false;
    }

    if (isZero(dir.squaredNorm())) {
      return false;
    }
  }

  return false;
}

/// GJK-based separation vector computation (stub)
template <typename S>
bool gjkSeparate(
    const void* /*obj1*/,
    const void* /*obj2*/,
    const CcdConfig<S>& /*config*/,
    Eigen::Vector3<S>& separation)
{
  separation.setZero();
  return false;
}

/// GJK+EPA penetration depth computation (stub)
template <typename S>
bool gjkPenetration(
    const void* /*obj1*/,
    const void* /*obj2*/,
    const CcdConfig<S>& /*config*/,
    S* depth,
    Eigen::Vector3<S>* direction,
    Eigen::Vector3<S>* position)
{
  if (depth)
    *depth = S(0);
  if (direction)
    direction->setZero();
  if (position)
    position->setZero();
  return false;
}

//==============================================================================
/// MPR Algorithm Helper Functions
//==============================================================================

/// Find origin (center) of Minkowski difference
template <typename S>
inline void findOrigin(
    const void* obj1,
    const void* obj2,
    const CcdConfig<S>& config,
    SupportPoint<S>& center)
{
  config.center1(obj1, center.v1);
  config.center2(obj2, center.v2);
  center.v = center.v1 - center.v2;
}

/// Fill direction with direction outside portal (from v1-v2-v3 face)
template <typename S>
inline void portalDir(const Simplex<S>& portal, Eigen::Vector3<S>& dir)
{
  auto v2v1 = portal[2].v - portal[1].v;
  auto v3v1 = portal[3].v - portal[1].v;
  dir = v2v1.cross(v3v1);
  dir.normalize();
}

/// Check if portal encapsulates origin
template <typename S>
inline bool portalEncapsulesOrigin(
    const Simplex<S>& portal, const Eigen::Vector3<S>& dir)
{
  S dot = dir.dot(portal[1].v);
  return isZero(dot) || dot > S(0);
}

/// Check if portal reached tolerance
template <typename S>
inline bool portalReachTolerance(
    const Simplex<S>& portal,
    const SupportPoint<S>& v4,
    const Eigen::Vector3<S>& dir,
    const CcdConfig<S>& config)
{
  S dv1 = portal[1].v.dot(dir);
  S dv2 = portal[2].v.dot(dir);
  S dv3 = portal[3].v.dot(dir);
  S dv4 = v4.v.dot(dir);

  S dot1 = dv4 - dv1;
  S dot2 = dv4 - dv2;
  S dot3 = dv4 - dv3;

  dot1 = std::min(dot1, dot2);
  dot1 = std::min(dot1, dot3);

  return isEqual(dot1, config.mprTolerance) || dot1 < config.mprTolerance;
}

/// Check if portal can encapsulate origin
template <typename S>
inline bool portalCanEncapsuleOrigin(
    const Simplex<S>& /*portal*/,
    const SupportPoint<S>& v4,
    const Eigen::Vector3<S>& dir)
{
  S dot = v4.v.dot(dir);
  return isZero(dot) || dot > S(0);
}

/// Expand portal with new support point
template <typename S>
inline void expandPortal(Simplex<S>& portal, const SupportPoint<S>& v4)
{
  auto v4v0 = v4.v.cross(portal[0].v);
  S dot = portal[1].v.dot(v4v0);

  if (dot > S(0)) {
    dot = portal[2].v.dot(v4v0);
    if (dot > S(0)) {
      portal[1] = v4;
    } else {
      portal[3] = v4;
    }
  } else {
    dot = portal[3].v.dot(v4v0);
    if (dot > S(0)) {
      portal[2] = v4;
    } else {
      portal[1] = v4;
    }
  }
}

/// Discover initial portal
template <typename S>
int discoverPortal(
    const void* obj1,
    const void* obj2,
    const CcdConfig<S>& config,
    Simplex<S>& portal)
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 dir, va, vb;
  S dot;

  // Vertex 0 is center of portal
  findOrigin(obj1, obj2, config, portal[0]);
  portal.setSize(1);

  // Check if center is at origin
  if (isEqual(portal[0].v, Vector3::Zero())) {
    va = Vector3(S(1e-8), S(0), S(0));
    portal[0].v += va;
  }

  // Vertex 1 = support in direction of origin
  dir = -portal[0].v;
  dir.normalize();
  computeSupport(obj1, obj2, dir, config, portal[1]);
  portal.setSize(2);

  // Test if origin isn't outside of v1
  dot = portal[1].v.dot(dir);
  if (isZero(dot) || dot < S(0))
    return -1;

  // Vertex 2
  dir = portal[0].v.cross(portal[1].v);
  if (isZero(dir.squaredNorm())) {
    if (isEqual(portal[1].v, Vector3::Zero())) {
      return 1; // origin lies on v1
    } else {
      return 2; // origin lies on v0-v1 segment
    }
  }

  dir.normalize();
  computeSupport(obj1, obj2, dir, config, portal[2]);
  dot = portal[2].v.dot(dir);
  if (isZero(dot) || dot < S(0))
    return -1;

  portal.setSize(3);

  // Vertex 3 direction
  va = portal[1].v - portal[0].v;
  vb = portal[2].v - portal[0].v;
  dir = va.cross(vb);
  dir.normalize();

  // Orient portal faces "outside" origin
  dot = dir.dot(portal[0].v);
  if (dot > S(0)) {
    std::swap(portal[1], portal[2]);
    dir = -dir;
  }

  while (portal.size() < 4) {
    computeSupport(obj1, obj2, dir, config, portal[3]);
    dot = portal[3].v.dot(dir);
    if (isZero(dot) || dot < S(0))
      return -1;

    bool cont = false;

    // Test if origin is outside (v1, v0, v3)
    va = portal[1].v.cross(portal[3].v);
    dot = va.dot(portal[0].v);
    if (dot < S(0) && !isZero(dot)) {
      portal[2] = portal[3];
      cont = true;
    }

    if (!cont) {
      // Test if origin is outside (v3, v0, v2)
      va = portal[3].v.cross(portal[2].v);
      dot = va.dot(portal[0].v);
      if (dot < S(0) && !isZero(dot)) {
        portal[1] = portal[3];
        cont = true;
      }
    }

    if (cont) {
      va = portal[1].v - portal[0].v;
      vb = portal[2].v - portal[0].v;
      dir = va.cross(vb);
      dir.normalize();
    } else {
      portal.setSize(4);
    }
  }

  return 0;
}

/// Refine portal
template <typename S>
int refinePortal(
    const void* obj1,
    const void* obj2,
    const CcdConfig<S>& config,
    Simplex<S>& portal)
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 dir;
  SupportPoint<S> v4;

  while (true) {
    portalDir(portal, dir);

    if (portalEncapsulesOrigin(portal, dir))
      return 0;

    computeSupport(obj1, obj2, dir, config, v4);

    if (!portalCanEncapsuleOrigin(portal, v4, dir)
        || portalReachTolerance(portal, v4, dir, config)) {
      return -1;
    }

    expandPortal(portal, v4);
  }

  return -1;
}

/// Compute distance from point to triangle
template <typename S, typename Derived1, typename Derived2, typename Derived3, typename Derived4>
S pointTriDist(
    const Eigen::MatrixBase<Derived1>& P,
    const Eigen::MatrixBase<Derived2>& v1,
    const Eigen::MatrixBase<Derived3>& v2,
    const Eigen::MatrixBase<Derived4>& v3,
    Eigen::Vector3<S>* witness)
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 B = v1 - P;
  Vector3 E0 = v2 - v1;
  Vector3 E1 = v3 - v1;

  S a = E0.dot(E0);
  S b = E0.dot(E1);
  S c = E1.dot(E1);
  S d = E0.dot(B);
  S e = E1.dot(B);

  S det = a * c - b * b;
  S s = b * e - c * d;
  S t = b * d - a * e;

  if (s + t <= det) {
    if (s < S(0)) {
      if (t < S(0)) {
        // Region 4
        if (d < S(0)) {
          t = S(0);
          s = (d >= S(0) ? S(0) : (-d >= a ? S(1) : -d / a));
        } else {
          s = S(0);
          t = (e >= S(0) ? S(0) : (-e >= c ? S(1) : -e / c));
        }
      } else {
        // Region 3
        s = S(0);
        t = (e >= S(0) ? S(0) : (-e >= c ? S(1) : -e / c));
      }
    } else if (t < S(0)) {
      // Region 5
      t = S(0);
      s = (d >= S(0) ? S(0) : (-d >= a ? S(1) : -d / a));
    } else {
      // Region 0
      S invDet = S(1) / det;
      s *= invDet;
      t *= invDet;
    }
  } else {
    if (s < S(0)) {
      // Region 2
      S tmp0 = b + d;
      S tmp1 = c + e;
      if (tmp1 > tmp0) {
        S numer = tmp1 - tmp0;
        S denom = a - S(2) * b + c;
        s = (numer >= denom ? S(1) : numer / denom);
        t = S(1) - s;
      } else {
        s = S(0);
        t = (tmp1 <= S(0) ? S(1) : (e >= S(0) ? S(0) : -e / c));
      }
    } else if (t < S(0)) {
      // Region 6
      S tmp0 = b + e;
      S tmp1 = a + d;
      if (tmp1 > tmp0) {
        S numer = tmp1 - tmp0;
        S denom = a - S(2) * b + c;
        t = (numer >= denom ? S(1) : numer / denom);
        s = S(1) - t;
      } else {
        t = S(0);
        s = (tmp1 <= S(0) ? S(1) : (d >= S(0) ? S(0) : -d / a));
      }
    } else {
      // Region 1
      S numer = c + e - b - d;
      if (numer <= S(0)) {
        s = S(0);
      } else {
        S denom = a - S(2) * b + c;
        s = (numer >= denom ? S(1) : numer / denom);
      }
      t = S(1) - s;
    }
  }

  if (witness) {
    *witness = v1 + s * E0 + t * E1 - P;
  }

  Vector3 diff = B + s * E0 + t * E1;
  return diff.norm();
}

/// Find position using barycentric coordinates
template <typename S>
void findPos(
    const void* /*obj1*/,
    const void* /*obj2*/,
    const CcdConfig<S>& /*config*/,
    const Simplex<S>& portal,
    Eigen::Vector3<S>& pos)
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 dir;
  portalDir(portal, dir);

  // Barycentric coordinates
  S b[4];
  Vector3 vec;

  vec = portal[1].v.cross(portal[2].v);
  b[0] = vec.dot(portal[3].v);

  vec = portal[3].v.cross(portal[2].v);
  b[1] = vec.dot(portal[0].v);

  vec = portal[0].v.cross(portal[1].v);
  b[2] = vec.dot(portal[3].v);

  vec = portal[2].v.cross(portal[1].v);
  b[3] = vec.dot(portal[0].v);

  S sum = b[0] + b[1] + b[2] + b[3];

  if (isZero(sum) || sum < S(0)) {
    b[0] = S(0);

    vec = portal[2].v.cross(portal[3].v);
    b[1] = vec.dot(dir);
    vec = portal[3].v.cross(portal[1].v);
    b[2] = vec.dot(dir);
    vec = portal[1].v.cross(portal[2].v);
    b[3] = vec.dot(dir);

    sum = b[1] + b[2] + b[3];
  }

  S inv = S(1) / sum;

  Vector3 p1 = Vector3::Zero();
  Vector3 p2 = Vector3::Zero();
  for (size_t i = 0; i < 4; i++) {
    p1 += b[i] * portal[i].v1;
    p2 += b[i] * portal[i].v2;
  }
  p1 *= inv;
  p2 *= inv;

  pos = (p1 + p2) * S(0.5);
}

/// Find penetration depth for touching contact
template <typename S>
void findPenetrTouch(
    const void* /*obj1*/,
    const void* /*obj2*/,
    const CcdConfig<S>& /*config*/,
    const Simplex<S>& portal,
    S& depth,
    Eigen::Vector3<S>& dir,
    Eigen::Vector3<S>& pos)
{
  depth = S(0);
  dir.setZero();
  pos = (portal[1].v1 + portal[1].v2) * S(0.5);
}

/// Find penetration depth for segment contact
template <typename S>
void findPenetrSegment(
    const void* /*obj1*/,
    const void* /*obj2*/,
    const CcdConfig<S>& /*config*/,
    const Simplex<S>& portal,
    S& depth,
    Eigen::Vector3<S>& dir,
    Eigen::Vector3<S>& pos)
{
  pos = (portal[1].v1 + portal[1].v2) * S(0.5);
  dir = portal[1].v;
  depth = dir.norm();
  if (depth > S(0))
    dir.normalize();
}

/// Find penetration info
template <typename S>
void findPenetr(
    const void* obj1,
    const void* obj2,
    const CcdConfig<S>& config,
    Simplex<S>& portal,
    S& depth,
    Eigen::Vector3<S>& pdir,
    Eigen::Vector3<S>& pos)
{
  using Vector3 = Eigen::Vector3<S>;

  Vector3 dir;
  SupportPoint<S> v4;
  unsigned long iterations = 0;

  while (true) {
    portalDir(portal, dir);
    computeSupport(obj1, obj2, dir, config, v4);

    if (portalReachTolerance(portal, v4, dir, config)
        || iterations > config.maxIterations) {
      depth = pointTriDist(
          Vector3::Zero(), portal[1].v, portal[2].v, portal[3].v, &pdir);

      if (isZero(depth)) {
        pdir.setZero();
      } else {
        pdir.normalize();
      }

      findPos(obj1, obj2, config, portal, pos);
      return;
    }

    expandPortal(portal, v4);
    iterations++;
  }
}

//==============================================================================
/// MPR-based intersection test
//==============================================================================
template <typename S>
bool mprIntersect(
    const void* obj1, const void* obj2, const CcdConfig<S>& config)
{
  Simplex<S> portal;

  // Phase 1: Portal discovery
  int res = discoverPortal(obj1, obj2, config, portal);
  if (res < 0)
    return false;
  if (res > 0)
    return true;

  // Phase 2: Portal refinement
  res = refinePortal(obj1, obj2, config, portal);
  return (res == 0);
}

//==============================================================================
/// MPR-based penetration depth computation
//==============================================================================
template <typename S>
bool mprPenetration(
    const void* obj1,
    const void* obj2,
    const CcdConfig<S>& config,
    S* depth,
    Eigen::Vector3<S>* direction,
    Eigen::Vector3<S>* position)
{
  Simplex<S> portal;

  // Phase 1: Portal discovery
  int res = discoverPortal(obj1, obj2, config, portal);
  if (res < 0) {
    // No collision
    return false;
  } else if (res == 1) {
    // Touching contact on portal's v1
    S d;
    Eigen::Vector3<S> dir, pos;
    findPenetrTouch(obj1, obj2, config, portal, d, dir, pos);
    if (depth)
      *depth = d;
    if (direction)
      *direction = dir;
    if (position)
      *position = pos;
  } else if (res == 2) {
    // Origin lies on v0-v1 segment
    S d;
    Eigen::Vector3<S> dir, pos;
    findPenetrSegment(obj1, obj2, config, portal, d, dir, pos);
    if (depth)
      *depth = d;
    if (direction)
      *direction = dir;
    if (position)
      *position = pos;
  } else if (res == 0) {
    // Phase 2: Portal refinement
    res = refinePortal(obj1, obj2, config, portal);
    if (res < 0)
      return false;

    // Phase 3: Penetration info
    S d;
    Eigen::Vector3<S> dir, pos;
    findPenetr(obj1, obj2, config, portal, d, dir, pos);
    if (depth)
      *depth = d;
    if (direction)
      *direction = dir;
    if (position)
      *position = pos;
  }

  return true;
}

} // namespace dart::collision::ccd

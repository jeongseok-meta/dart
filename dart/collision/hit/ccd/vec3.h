/***
 * libccd
 * ---------------------------------
 * Copyright (c)2010-2013 Daniel Fiser <danfis@danfis.cz>
 *
 *
 *  This file is part of libccd.
 *
 *  Distributed under the OSI-approved BSD License (the "License");
 *  see accompanying file BDS-LICENSE for details or see
 *  <http://www.opensource.org/licenses/bsd-license.php>.
 *
 *  This software is distributed WITHOUT ANY WARRANTY; without even the
 *  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the License for more information.
 */

#pragma once

#include "dart/collision/hit/ccd/compiler.h"

#include <float.h>
#include <math.h>
#include <stdlib.h>

/* Default to double precision */
#ifndef CCD_SINGLE
#ifndef CCD_DOUBLE
#define CCD_DOUBLE
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CCD_SINGLE
  #ifndef CCD_DOUBLE
    #error You must define CCD_SINGLE or CCD_DOUBLE
  #endif /* CCD_DOUBLE */
#endif   /* CCD_SINGLE */

#ifdef WIN32
  #define DART_CCD_FMIN(x, y) ((x) < (y) ? (x) : (y))
#endif /* WIN32 */

#ifdef CCD_SINGLE
  #ifdef CCD_DOUBLE
    #error You can define either CCD_SINGLE or CCD_DOUBLE, not both!
  #endif /* CCD_DOUBLE */

typedef float dart_ccd_real_t;

  //# define CCD_EPS 1E-6
  #define CCD_EPS FLT_EPSILON

  #define CCD_REAL_MAX FLT_MAX

  #define DART_CCD_REAL(x) (x##f)               /*!< form a constant */
  #define DART_CCD_SQRT(x) (sqrtf(x))           /*!< square root */
  #define DART_CCD_FABS(x) (fabsf(x))           /*!< absolute value */
  #define DART_CCD_FMAX(x, y) (fmaxf((x), (y))) /*!< maximum of two floats */

  #ifndef DART_CCD_FMIN
    #define DART_CCD_FMIN(x, y) (fminf((x), (y))) /*!< minimum of two floats   \
                                                   */
  #endif                                          /* DART_CCD_FMIN */

#endif /* CCD_SINGLE */

#ifdef CCD_DOUBLE
typedef double dart_ccd_real_t;

  //# define CCD_EPS 1E-10
  #define CCD_EPS DBL_EPSILON

  #define CCD_REAL_MAX DBL_MAX

  #define DART_CCD_REAL(x) (x)                 /*!< form a constant */
  #define DART_CCD_SQRT(x) (sqrt(x))           /*!< square root */
  #define DART_CCD_FABS(x) (fabs(x))           /*!< absolute value */
  #define DART_CCD_FMAX(x, y) (fmax((x), (y))) /*!< maximum of two floats */

  #ifndef DART_CCD_FMIN
    #define DART_CCD_FMIN(x, y) (fmin((x), (y))) /*!< minimum of two floats */
  #endif                                         /* DART_CCD_FMIN */

#endif /* CCD_DOUBLE */

#define CCD_ONE DART_CCD_REAL(1.)
#define CCD_ZERO DART_CCD_REAL(0.)

struct _dart_ccd_vec3_t
{
  dart_ccd_real_t v[3];
};
typedef struct _dart_ccd_vec3_t dart_ccd_vec3_t;

/**
 * Holds origin (0,0,0) - this variable is meant to be read-only!
 */
extern dart_ccd_vec3_t* ccd_vec3_origin;

/**
 * Array of points uniformly distributed on unit sphere.
 */
extern dart_ccd_vec3_t* ccd_points_on_sphere;
extern size_t ccd_points_on_sphere_len;

/** Returns sign of value. */
inline int ccdSign(dart_ccd_real_t val);
/** Returns true if val is zero. **/
inline int ccdIsZero(dart_ccd_real_t val);
/** Returns true if a and b equal. **/
inline int ccdEq(dart_ccd_real_t a, dart_ccd_real_t b);

#define CCD_VEC3_STATIC(x, y, z)                                               \
  {                                                                            \
    {                                                                          \
      (x), (y), (z)                                                            \
    }                                                                          \
  }

#define CCD_VEC3(name, x, y, z)                                                \
  dart_ccd_vec3_t name = CCD_VEC3_STATIC((x), (y), (z))

inline dart_ccd_real_t dartCcdVec3X(const dart_ccd_vec3_t* v);
inline dart_ccd_real_t dartCcdVec3Y(const dart_ccd_vec3_t* v);
inline dart_ccd_real_t dartCcdVec3Z(const dart_ccd_vec3_t* v);

/**
 * Returns true if a and b equal.
 */
inline int dartCcdVec3Eq(
    const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b);

/**
 * Returns squared length of vector.
 */
inline dart_ccd_real_t dartCcdVec3Len2(const dart_ccd_vec3_t* v);

/**
 * Returns distance between a and b.
 */
inline dart_ccd_real_t
dartCcdVec3Dist2(const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b);

inline void dartCcdVec3Set(
    dart_ccd_vec3_t* v,
    dart_ccd_real_t x,
    dart_ccd_real_t y,
    dart_ccd_real_t z);

/**
 * v = w
 */
inline void dartCcdVec3Copy(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w);

/**
 * Substracts coordinates of vector w from vector v. v = v - w
 */
inline void dartCcdVec3Sub(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w);

/**
 * Adds coordinates of vector w to vector v. v = v + w
 */
inline void dartCcdVec3Add(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w);

/**
 * d = v - w
 */
inline void dartCcdVec3Sub2(
    dart_ccd_vec3_t* d, const dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w);

/**
 * d = d * k;
 */
inline void dartCcdVec3Scale(dart_ccd_vec3_t* d, dart_ccd_real_t k);

/**
 * Normalizes given vector to unit length.
 */
inline void dartCcdVec3Normalize(dart_ccd_vec3_t* d);

/**
 * Dot product of two vectors.
 */
inline dart_ccd_real_t
dartCcdVec3Dot(const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b);

/**
 * Cross product: d = a x b.
 */
inline void dartCcdVec3Cross(
    dart_ccd_vec3_t* d, const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b);

/**
 * Returns distance^2 of point P to segment ab.
 * If witness is non-NULL it is filled with coordinates of point from which
 * was computed distance to point P.
 */
dart_ccd_real_t dartCcdVec3PointSegmentDist2(
    const dart_ccd_vec3_t* P,
    const dart_ccd_vec3_t* a,
    const dart_ccd_vec3_t* b,
    dart_ccd_vec3_t* witness);

/**
 * Returns distance^2 of point P from triangle formed by triplet a, b, c.
 * If witness vector is provided it is filled with coordinates of point
 * from which was computed distance to point P.
 */
dart_ccd_real_t dartCcdVec3PointTriDist2(
    const dart_ccd_vec3_t* P,
    const dart_ccd_vec3_t* a,
    const dart_ccd_vec3_t* b,
    const dart_ccd_vec3_t* c,
    dart_ccd_vec3_t* witness);

/**** INLINES ****/
inline int ccdSign(dart_ccd_real_t val)
{
  if (ccdIsZero(val)) {
    return 0;
  } else if (val < CCD_ZERO) {
    return -1;
  }
  return 1;
}

inline int ccdIsZero(dart_ccd_real_t val)
{
  return DART_CCD_FABS(val) < CCD_EPS;
}

inline int ccdEq(dart_ccd_real_t _a, dart_ccd_real_t _b)
{
  dart_ccd_real_t ab;
  dart_ccd_real_t a, b;

  ab = DART_CCD_FABS(_a - _b);
  if (DART_CCD_FABS(ab) < CCD_EPS)
    return 1;

  a = DART_CCD_FABS(_a);
  b = DART_CCD_FABS(_b);
  if (b > a) {
    return ab < CCD_EPS * b;
  } else {
    return ab < CCD_EPS * a;
  }
}

inline dart_ccd_real_t dartCcdVec3X(const dart_ccd_vec3_t* v)
{
  return v->v[0];
}

inline dart_ccd_real_t dartCcdVec3Y(const dart_ccd_vec3_t* v)
{
  return v->v[1];
}

inline dart_ccd_real_t dartCcdVec3Z(const dart_ccd_vec3_t* v)
{
  return v->v[2];
}

inline int dartCcdVec3Eq(
    const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b)
{
  return ccdEq(dartCcdVec3X(a), dartCcdVec3X(b))
         && ccdEq(dartCcdVec3Y(a), dartCcdVec3Y(b))
         && ccdEq(dartCcdVec3Z(a), dartCcdVec3Z(b));
}

inline dart_ccd_real_t dartCcdVec3Len2(const dart_ccd_vec3_t* v)
{
  return dartCcdVec3Dot(v, v);
}

inline dart_ccd_real_t
dartCcdVec3Dist2(const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b)
{
  dart_ccd_vec3_t ab;
  dartCcdVec3Sub2(&ab, a, b);
  return dartCcdVec3Len2(&ab);
}

inline void dartCcdVec3Set(
    dart_ccd_vec3_t* v, dart_ccd_real_t x, dart_ccd_real_t y, dart_ccd_real_t z)
{
  v->v[0] = x;
  v->v[1] = y;
  v->v[2] = z;
}

inline void dartCcdVec3Copy(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w)
{
  *v = *w;
}

inline void dartCcdVec3Sub(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w)
{
  v->v[0] -= w->v[0];
  v->v[1] -= w->v[1];
  v->v[2] -= w->v[2];
}
inline void dartCcdVec3Sub2(
    dart_ccd_vec3_t* d, const dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w)
{
  d->v[0] = v->v[0] - w->v[0];
  d->v[1] = v->v[1] - w->v[1];
  d->v[2] = v->v[2] - w->v[2];
}

inline void dartCcdVec3Add(
    dart_ccd_vec3_t* v, const dart_ccd_vec3_t* w)
{
  v->v[0] += w->v[0];
  v->v[1] += w->v[1];
  v->v[2] += w->v[2];
}

inline void dartCcdVec3Scale(dart_ccd_vec3_t* d, dart_ccd_real_t k)
{
  d->v[0] *= k;
  d->v[1] *= k;
  d->v[2] *= k;
}

inline void dartCcdVec3Normalize(dart_ccd_vec3_t* d)
{
  dart_ccd_real_t k = CCD_ONE / DART_CCD_SQRT(dartCcdVec3Len2(d));
  dartCcdVec3Scale(d, k);
}

inline dart_ccd_real_t
dartCcdVec3Dot(const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b)
{
  dart_ccd_real_t dot;

  dot = a->v[0] * b->v[0];
  dot += a->v[1] * b->v[1];
  dot += a->v[2] * b->v[2];
  return dot;
}

inline void dartCcdVec3Cross(
    dart_ccd_vec3_t* d, const dart_ccd_vec3_t* a, const dart_ccd_vec3_t* b)
{
  d->v[0] = (a->v[1] * b->v[2]) - (a->v[2] * b->v[1]);
  d->v[1] = (a->v[2] * b->v[0]) - (a->v[0] * b->v[2]);
  d->v[2] = (a->v[0] * b->v[1]) - (a->v[1] * b->v[0]);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

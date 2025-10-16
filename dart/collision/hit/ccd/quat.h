/***
 * libccd
 * ---------------------------------
 * Copyright (c)2010 Daniel Fiser <danfis@danfis.cz>
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
#include "dart/collision/hit/ccd/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct _dart_ccd_quat_t
{
  dart_ccd_real_t q[4]; //!< x, y, z, w
};
typedef struct _dart_ccd_quat_t dart_ccd_quat_t;

#define CCD_QUAT(name, x, y, z, w) dart_ccd_quat_t name = {{x, y, z, w}}

inline dart_ccd_real_t dartCcdQuatLen2(const dart_ccd_quat_t* q);
inline dart_ccd_real_t dartCcdQuatLen(const dart_ccd_quat_t* q);

inline void dartCcdQuatSet(
    dart_ccd_quat_t* q,
    dart_ccd_real_t x,
    dart_ccd_real_t y,
    dart_ccd_real_t z,
    dart_ccd_real_t w);
inline void dartCcdQuatCopy(
    dart_ccd_quat_t* dest, const dart_ccd_quat_t* src);

inline int dartCcdQuatNormalize(dart_ccd_quat_t* q);

inline void dartCcdQuatSetAngleAxis(
    dart_ccd_quat_t* q, dart_ccd_real_t angle, const dart_ccd_vec3_t* axis);

inline void dartCcdQuatScale(dart_ccd_quat_t* q, dart_ccd_real_t k);

/**
 * q = q * q2
 */
inline void dartCcdQuatMul(
    dart_ccd_quat_t* q, const dart_ccd_quat_t* q2);

/**
 * q = a * b
 */
inline void dartCcdQuatMul2(
    dart_ccd_quat_t* q, const dart_ccd_quat_t* a, const dart_ccd_quat_t* b);

/**
 * Inverts quaternion.
 * Returns 0 on success.
 */
inline int dartCcdQuatInvert(dart_ccd_quat_t* q);
inline int dartCcdQuatInvert2(
    dart_ccd_quat_t* dest, const dart_ccd_quat_t* src);

/**
 * Rotate vector v by quaternion q.
 */
inline void dartCcdQuatRotVec(
    dart_ccd_vec3_t* v, const dart_ccd_quat_t* q);

/**** INLINES ****/
inline dart_ccd_real_t dartCcdQuatLen2(const dart_ccd_quat_t* q)
{
  dart_ccd_real_t len;

  len = q->q[0] * q->q[0];
  len += q->q[1] * q->q[1];
  len += q->q[2] * q->q[2];
  len += q->q[3] * q->q[3];

  return len;
}

inline dart_ccd_real_t dartCcdQuatLen(const dart_ccd_quat_t* q)
{
  return DART_CCD_SQRT(dartCcdQuatLen2(q));
}

inline void dartCcdQuatSet(
    dart_ccd_quat_t* q,
    dart_ccd_real_t x,
    dart_ccd_real_t y,
    dart_ccd_real_t z,
    dart_ccd_real_t w)
{
  q->q[0] = x;
  q->q[1] = y;
  q->q[2] = z;
  q->q[3] = w;
}

inline void dartCcdQuatCopy(
    dart_ccd_quat_t* dest, const dart_ccd_quat_t* src)
{
  *dest = *src;
}

inline int dartCcdQuatNormalize(dart_ccd_quat_t* q)
{
  dart_ccd_real_t len = dartCcdQuatLen(q);
  if (len < CCD_EPS)
    return 0;

  dartCcdQuatScale(q, CCD_ONE / len);
  return 1;
}

inline void dartCcdQuatSetAngleAxis(
    dart_ccd_quat_t* q, dart_ccd_real_t angle, const dart_ccd_vec3_t* axis)
{
  dart_ccd_real_t a, x, y, z, n, s;

  a = angle / 2;
  x = dartCcdVec3X(axis);
  y = dartCcdVec3Y(axis);
  z = dartCcdVec3Z(axis);
  n = DART_CCD_SQRT(x * x + y * y + z * z);

  // axis==0? (treat this the same as angle==0 with an arbitrary axis)
  if (n < CCD_EPS) {
    q->q[0] = q->q[1] = q->q[2] = CCD_ZERO;
    q->q[3] = CCD_ONE;
  } else {
    s = sin(a) / n;

    q->q[3] = cos(a);
    q->q[0] = x * s;
    q->q[1] = y * s;
    q->q[2] = z * s;

    dartCcdQuatNormalize(q);
  }
}

inline void dartCcdQuatScale(dart_ccd_quat_t* q, dart_ccd_real_t k)
{
  size_t i;
  for (i = 0; i < 4; i++)
    q->q[i] *= k;
}

inline void dartCcdQuatMul(
    dart_ccd_quat_t* q, const dart_ccd_quat_t* q2)
{
  dart_ccd_quat_t a;
  dartCcdQuatCopy(&a, q);
  dartCcdQuatMul2(q, &a, q2);
}

inline void dartCcdQuatMul2(
    dart_ccd_quat_t* q, const dart_ccd_quat_t* a, const dart_ccd_quat_t* b)
{
  q->q[0] = a->q[3] * b->q[0] + a->q[0] * b->q[3] + a->q[1] * b->q[2]
            - a->q[2] * b->q[1];
  q->q[1] = a->q[3] * b->q[1] + a->q[1] * b->q[3] - a->q[0] * b->q[2]
            + a->q[2] * b->q[0];
  q->q[2] = a->q[3] * b->q[2] + a->q[2] * b->q[3] + a->q[0] * b->q[1]
            - a->q[1] * b->q[0];
  q->q[3] = a->q[3] * b->q[3] - a->q[0] * b->q[0] - a->q[1] * b->q[1]
            - a->q[2] * b->q[2];
}

inline int dartCcdQuatInvert(dart_ccd_quat_t* q)
{
  dart_ccd_real_t len2 = dartCcdQuatLen2(q);
  if (len2 < CCD_EPS)
    return -1;

  len2 = CCD_ONE / len2;

  q->q[0] = -q->q[0] * len2;
  q->q[1] = -q->q[1] * len2;
  q->q[2] = -q->q[2] * len2;
  q->q[3] = q->q[3] * len2;

  return 0;
}
inline int dartCcdQuatInvert2(
    dart_ccd_quat_t* dest, const dart_ccd_quat_t* src)
{
  dartCcdQuatCopy(dest, src);
  return dartCcdQuatInvert(dest);
}

inline void dartCcdQuatRotVec(
    dart_ccd_vec3_t* v, const dart_ccd_quat_t* q)
{
  // original version: 31 mul + 21 add
  // optimized version: 18 mul + 12 add
  // formula: v = v + 2 * cross(q.xyz, cross(q.xyz, v) + q.w * v)
  dart_ccd_real_t cross1_x, cross1_y, cross1_z, cross2_x, cross2_y, cross2_z;
  dart_ccd_real_t x, y, z, w;
  dart_ccd_real_t vx, vy, vz;

  vx = dartCcdVec3X(v);
  vy = dartCcdVec3Y(v);
  vz = dartCcdVec3Z(v);

  w = q->q[3];
  x = q->q[0];
  y = q->q[1];
  z = q->q[2];

  cross1_x = y * vz - z * vy + w * vx;
  cross1_y = z * vx - x * vz + w * vy;
  cross1_z = x * vy - y * vx + w * vz;
  cross2_x = y * cross1_z - z * cross1_y;
  cross2_y = z * cross1_x - x * cross1_z;
  cross2_z = x * cross1_y - y * cross1_x;
  dartCcdVec3Set(v, vx + 2 * cross2_x, vy + 2 * cross2_y, vz + 2 * cross2_z);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

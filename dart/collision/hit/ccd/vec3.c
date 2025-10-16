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

#include <stdio.h>
#include "dart/collision/hit/ccd/vec3.h"
#include "dbg.h"

static CCD_VEC3(__ccd_vec3_origin, CCD_ZERO, CCD_ZERO, CCD_ZERO);
dart_ccd_vec3_t *ccd_vec3_origin = &__ccd_vec3_origin;

static dart_ccd_vec3_t points_on_sphere[] = {
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.000000), DART_CCD_REAL(-0.000000), DART_CCD_REAL(-1.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.723608), DART_CCD_REAL(-0.525725), DART_CCD_REAL(-0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.276388), DART_CCD_REAL(-0.850649), DART_CCD_REAL(-0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.894426), DART_CCD_REAL(-0.000000), DART_CCD_REAL(-0.447216)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.276388), DART_CCD_REAL( 0.850649), DART_CCD_REAL(-0.447220)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.723608), DART_CCD_REAL( 0.525725), DART_CCD_REAL(-0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.276388), DART_CCD_REAL(-0.850649), DART_CCD_REAL( 0.447220)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.723608), DART_CCD_REAL(-0.525725), DART_CCD_REAL( 0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.723608), DART_CCD_REAL( 0.525725), DART_CCD_REAL( 0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.276388), DART_CCD_REAL( 0.850649), DART_CCD_REAL( 0.447219)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.894426), DART_CCD_REAL( 0.000000), DART_CCD_REAL( 0.447216)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.000000), DART_CCD_REAL( 0.000000), DART_CCD_REAL( 1.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.425323), DART_CCD_REAL(-0.309011), DART_CCD_REAL(-0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.162456), DART_CCD_REAL(-0.499995), DART_CCD_REAL(-0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.262869), DART_CCD_REAL(-0.809012), DART_CCD_REAL(-0.525738)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.425323), DART_CCD_REAL( 0.309011), DART_CCD_REAL(-0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.850648), DART_CCD_REAL(-0.000000), DART_CCD_REAL(-0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.525730), DART_CCD_REAL(-0.000000), DART_CCD_REAL(-0.850652)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.688190), DART_CCD_REAL(-0.499997), DART_CCD_REAL(-0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.162456), DART_CCD_REAL( 0.499995), DART_CCD_REAL(-0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.688190), DART_CCD_REAL( 0.499997), DART_CCD_REAL(-0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.262869), DART_CCD_REAL( 0.809012), DART_CCD_REAL(-0.525738)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.951058), DART_CCD_REAL( 0.309013), DART_CCD_REAL( 0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.951058), DART_CCD_REAL(-0.309013), DART_CCD_REAL( 0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.587786), DART_CCD_REAL(-0.809017), DART_CCD_REAL( 0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.000000), DART_CCD_REAL(-1.000000), DART_CCD_REAL( 0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.587786), DART_CCD_REAL(-0.809017), DART_CCD_REAL( 0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.951058), DART_CCD_REAL(-0.309013), DART_CCD_REAL(-0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.951058), DART_CCD_REAL( 0.309013), DART_CCD_REAL(-0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.587786), DART_CCD_REAL( 0.809017), DART_CCD_REAL(-0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.000000), DART_CCD_REAL( 1.000000), DART_CCD_REAL(-0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.587786), DART_CCD_REAL( 0.809017), DART_CCD_REAL(-0.000000)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.688190), DART_CCD_REAL(-0.499997), DART_CCD_REAL( 0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.262869), DART_CCD_REAL(-0.809012), DART_CCD_REAL( 0.525738)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.850648), DART_CCD_REAL( 0.000000), DART_CCD_REAL( 0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.262869), DART_CCD_REAL( 0.809012), DART_CCD_REAL( 0.525738)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.688190), DART_CCD_REAL( 0.499997), DART_CCD_REAL( 0.525736)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.525730), DART_CCD_REAL( 0.000000), DART_CCD_REAL( 0.850652)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.162456), DART_CCD_REAL(-0.499995), DART_CCD_REAL( 0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.425323), DART_CCD_REAL(-0.309011), DART_CCD_REAL( 0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL(-0.425323), DART_CCD_REAL( 0.309011), DART_CCD_REAL( 0.850654)),
	CCD_VEC3_STATIC(DART_CCD_REAL( 0.162456), DART_CCD_REAL( 0.499995), DART_CCD_REAL( 0.850654))
};
dart_ccd_vec3_t *ccd_points_on_sphere = points_on_sphere;
size_t ccd_points_on_sphere_len = sizeof(points_on_sphere) / sizeof(dart_ccd_vec3_t);


inline dart_ccd_real_t __ccdVec3PointSegmentDist2(const dart_ccd_vec3_t *P,
                                                  const dart_ccd_vec3_t *x0,
                                                  const dart_ccd_vec3_t *b,
                                                  dart_ccd_vec3_t *witness)
{
    // The computation comes from solving equation of segment:
    //      S(t) = x0 + t.d
    //          where - x0 is initial point of segment
    //                - d is direction of segment from x0 (|d| > 0)
    //                - t belongs to <0, 1> interval
    //
    // Than, distance from a segment to some point P can be expressed:
    //      D(t) = |x0 + t.d - P|^2
    //          which is distance from any point on segment. Minimization
    //          of this function brings distance from P to segment.
    // Minimization of D(t) leads to simple quadratic equation that's
    // solving is straightforward.
    //
    // Bonus of this method is witness point for free.

    dart_ccd_real_t dist, t;
    dart_ccd_vec3_t d, a;

    // direction of segment
    dartCcdVec3Sub2(&d, b, x0);

    // precompute vector from P to x0
    dartCcdVec3Sub2(&a, x0, P);

    t  = -DART_CCD_REAL(1.) * dartCcdVec3Dot(&a, &d);
    t /= dartCcdVec3Len2(&d);

    if (t < CCD_ZERO || ccdIsZero(t)){
        dist = dartCcdVec3Dist2(x0, P);
        if (witness)
            dartCcdVec3Copy(witness, x0);
    }else if (t > CCD_ONE || ccdEq(t, CCD_ONE)){
        dist = dartCcdVec3Dist2(b, P);
        if (witness)
            dartCcdVec3Copy(witness, b);
    }else{
        if (witness){
            dartCcdVec3Copy(witness, &d);
            dartCcdVec3Scale(witness, t);
            dartCcdVec3Add(witness, x0);
            dist = dartCcdVec3Dist2(witness, P);
        }else{
            // recycling variables
            dartCcdVec3Scale(&d, t);
            dartCcdVec3Add(&d, &a);
            dist = dartCcdVec3Len2(&d);
        }
    }

    return dist;
}

dart_ccd_real_t dartCcdVec3PointSegmentDist2(const dart_ccd_vec3_t *P,
                                    const dart_ccd_vec3_t *x0, const dart_ccd_vec3_t *b,
                                    dart_ccd_vec3_t *witness)
{
    return __ccdVec3PointSegmentDist2(P, x0, b, witness);
}

dart_ccd_real_t dartCcdVec3PointTriDist2(const dart_ccd_vec3_t *P,
                                const dart_ccd_vec3_t *x0, const dart_ccd_vec3_t *B,
                                const dart_ccd_vec3_t *C,
                                dart_ccd_vec3_t *witness)
{
    // Computation comes from analytic expression for triangle (x0, B, C)
    //      T(s, t) = x0 + s.d1 + t.d2, where d1 = B - x0 and d2 = C - x0 and
    // Then equation for distance is:
    //      D(s, t) = | T(s, t) - P |^2
    // This leads to minimization of quadratic function of two variables.
    // The solution from is taken only if s is between 0 and 1, t is
    // between 0 and 1 and t + s < 1, otherwise distance from segment is
    // computed.

    dart_ccd_vec3_t d1, d2, a;
    dart_ccd_real_t u, v, w, p, q, r, d;
    dart_ccd_real_t s, t, dist, dist2;
    dart_ccd_vec3_t witness2;

    dartCcdVec3Sub2(&d1, B, x0);
    dartCcdVec3Sub2(&d2, C, x0);
    dartCcdVec3Sub2(&a, x0, P);

    u = dartCcdVec3Dot(&a, &a);
    v = dartCcdVec3Dot(&d1, &d1);
    w = dartCcdVec3Dot(&d2, &d2);
    p = dartCcdVec3Dot(&a, &d1);
    q = dartCcdVec3Dot(&a, &d2);
    r = dartCcdVec3Dot(&d1, &d2);

    d = w * v - r * r;
    if (ccdIsZero(d)){
        // To avoid division by zero for zero (or near zero) area triangles
        s = t = -1.;
    }else{
        s = (q * r - w * p) / d;
        t = (-s * r - q) / w;
    }

    if ((ccdIsZero(s) || s > CCD_ZERO)
            && (ccdEq(s, CCD_ONE) || s < CCD_ONE)
            && (ccdIsZero(t) || t > CCD_ZERO)
            && (ccdEq(t, CCD_ONE) || t < CCD_ONE)
            && (ccdEq(t + s, CCD_ONE) || t + s < CCD_ONE)){

        if (witness){
            dartCcdVec3Scale(&d1, s);
            dartCcdVec3Scale(&d2, t);
            dartCcdVec3Copy(witness, x0);
            dartCcdVec3Add(witness, &d1);
            dartCcdVec3Add(witness, &d2);

            dist = dartCcdVec3Dist2(witness, P);
        }else{
            dist  = s * s * v;
            dist += t * t * w;
            dist += DART_CCD_REAL(2.) * s * t * r;
            dist += DART_CCD_REAL(2.) * s * p;
            dist += DART_CCD_REAL(2.) * t * q;
            dist += u;
        }
    }else{
        dist = __ccdVec3PointSegmentDist2(P, x0, B, witness);

        dist2 = __ccdVec3PointSegmentDist2(P, x0, C, &witness2);
        if (dist2 < dist){
            dist = dist2;
            if (witness)
                dartCcdVec3Copy(witness, &witness2);
        }

        dist2 = __ccdVec3PointSegmentDist2(P, B, C, &witness2);
        if (dist2 < dist){
            dist = dist2;
            if (witness)
                dartCcdVec3Copy(witness, &witness2);
        }
    }

    return dist;
}

/***
 * libccd
 * ---------------------------------
 * Copyright (c)2010,2011 Daniel Fiser <danfis@danfis.cz>
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

#include "dart/collision/hit/ccd/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Type of *support* function that takes pointer to 3D object and direction
 * and returns (via vec argument) furthest point from object in specified
 * direction.
 */
typedef void (*dart_ccd_support_fn)(
    const void* obj, const dart_ccd_vec3_t* dir, dart_ccd_vec3_t* vec);

/**
 * Returns (via dir argument) first direction vector that will be used in
 * initialization of algorithm.
 */
typedef void (*dart_ccd_first_dir_fn)(
    const void* obj1, const void* obj2, dart_ccd_vec3_t* dir);

/**
 * Returns (via center argument) geometric center (some point near center)
 * of given object.
 */
typedef void (*dart_ccd_center_fn)(const void* obj1, dart_ccd_vec3_t* center);

/**
 * Main structure of CCD algorithm.
 */
struct _dart_ccd_t
{
  dart_ccd_first_dir_fn first_dir; //!< Returns initial direction where first
                                   //!< support point will be searched
  dart_ccd_support_fn support1;    //!< Function that returns support point of
                                   //!< first object
  dart_ccd_support_fn support2;    //!< Function that returns support point of
                                   //!< second object

  dart_ccd_center_fn center1; //!< Function that returns geometric center of
                              //!< first object
  dart_ccd_center_fn center2; //!< Function that returns geometric center of
                              //!< second object

  unsigned long max_iterations; //!< Maximal number of iterations
  dart_ccd_real_t epa_tolerance;
  dart_ccd_real_t mpr_tolerance; //!< Boundary tolerance for MPR algorithm
  dart_ccd_real_t dist_tolerance;
};
typedef struct _dart_ccd_t dart_ccd_t;

/**
 * Default first direction.
 */
void dartCcdFirstDirDefault(
    const void* o1, const void* o2, dart_ccd_vec3_t* dir);

#define DART_CCD_INIT(ccd)                                                     \
  do {                                                                         \
    (ccd)->first_dir = dartCcdFirstDirDefault;                                 \
    (ccd)->support1 = NULL;                                                    \
    (ccd)->support2 = NULL;                                                    \
    (ccd)->center1 = NULL;                                                     \
    (ccd)->center2 = NULL;                                                     \
                                                                               \
    (ccd)->max_iterations = (unsigned long)-1;                                 \
    (ccd)->epa_tolerance = DART_CCD_REAL(0.0001);                              \
    (ccd)->mpr_tolerance = DART_CCD_REAL(0.0001);                              \
    (ccd)->dist_tolerance = DART_CCD_REAL(1E-6);                               \
  } while (0)

/**
 * Returns true if two given objects interest.
 */
int dartCcdGJKIntersect(
    const void* obj1, const void* obj2, const dart_ccd_t* ccd);

/**
 * This function computes separation vector of two objects. Separation
 * vector is minimal translation of obj2 to get obj1 and obj2 speparated
 * (without intersection).
 * Returns 0 if obj1 and obj2 intersect and sep is filled with translation
 * vector. If obj1 and obj2 don't intersect -1 is returned.
 * If memory allocation fails -2 is returned.
 */
int dartCcdGJKSeparate(
    const void* obj1,
    const void* obj2,
    const dart_ccd_t* ccd,
    dart_ccd_vec3_t* sep);

/**
 * Computes penetration of obj2 into obj1.
 * Depth of penetration, direction and position is returned. It means that
 * if obj2 is translated by distance depth in direction dir objects will
 * have touching contact, pos should be position in global coordinates
 * where force should take a place.
 *
 * CCD+EPA algorithm is used.
 *
 * Returns 0 if obj1 and obj2 intersect and depth, dir and pos are filled
 * if given non-NULL pointers.
 * If obj1 and obj2 don't intersect -1 is returned.
 * If memory allocation fails -2 is returned.
 */
int dartCcdGJKPenetration(
    const void* obj1,
    const void* obj2,
    const dart_ccd_t* ccd,
    dart_ccd_real_t* depth,
    dart_ccd_vec3_t* dir,
    dart_ccd_vec3_t* pos);

/**
 * Returns true if two given objects intersect - MPR algorithm is used.
 */
int dartCcdMPRIntersect(
    const void* obj1, const void* obj2, const dart_ccd_t* ccd);

/**
 * Computes penetration of obj2 into obj1.
 * Depth of penetration, direction and position is returned, i.e. if obj2
 * is translated by computed depth in resulting direction obj1 and obj2
 * would have touching contact. Position is point in global coordinates
 * where force should take a place.
 *
 * Minkowski Portal Refinement algorithm is used (MPR, a.k.a. XenoCollide,
 * see Game Programming Gem 7).
 *
 * Returns 0 if obj1 and obj2 intersect, otherwise -1 is returned.
 */
int dartCcdMPRPenetration(
    const void* obj1,
    const void* obj2,
    const dart_ccd_t* ccd,
    dart_ccd_real_t* depth,
    dart_ccd_vec3_t* dir,
    dart_ccd_vec3_t* pos);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

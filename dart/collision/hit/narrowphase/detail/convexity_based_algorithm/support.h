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

#include "dart/collision/hit/ccd/ccd.h"
#include "dart/collision/hit/ccd/vec3.h"

namespace dart::collision::hit::detail {

// Type aliases to map the renamed dart_ccd types back to ccd for compatibility
using ccd_t = dart_ccd_t;
using ccd_vec3_t = dart_ccd_vec3_t;
using ccd_support_fn = dart_ccd_support_fn;
using ccd_center_fn = dart_ccd_center_fn;
using ccd_real_t = dart_ccd_real_t;

// Function aliases to map original ccd function names to our renamed versions
#define ccdVec3X dartCcdVec3X
#define ccdVec3Y dartCcdVec3Y
#define ccdVec3Z dartCcdVec3Z
#define ccdVec3Set dartCcdVec3Set
#define ccdVec3Copy dartCcdVec3Copy
#define ccdVec3Sub dartCcdVec3Sub
#define ccdVec3Sub2 dartCcdVec3Sub2
#define ccdVec3Add dartCcdVec3Add
#define ccdVec3Scale dartCcdVec3Scale
#define ccdVec3Normalize dartCcdVec3Normalize
#define ccdVec3Dot dartCcdVec3Dot
#define ccdVec3Cross dartCcdVec3Cross
#define ccdVec3Len dartCcdVec3Len
#define ccdVec3Len2 dartCcdVec3Len2
#define ccdVec3Dist2 dartCcdVec3Dist2
#define ccdVec3Eq dartCcdVec3Eq
#define ccdVec3PointSegmentDist2 dartCcdVec3PointSegmentDist2
#define ccdVec3PointTriDist2 dartCcdVec3PointTriDist2

// Macro aliases
#define CCD_INIT DART_CCD_INIT
#define CCD_REAL DART_CCD_REAL
#define CCD_SQRT DART_CCD_SQRT
#define CCD_FABS DART_CCD_FABS
#define CCD_FMIN DART_CCD_FMIN
#define CCD_FMAX DART_CCD_FMAX

struct _ccd_support_t
{
  ccd_vec3_t v;  //!< Support point in minkowski sum
  ccd_vec3_t v1; //!< Support point in obj1
  ccd_vec3_t v2; //!< Support point in obj2
};
typedef struct _ccd_support_t ccd_support_t;

inline void ccdSupportCopy(ccd_support_t*, const ccd_support_t* s);

/**
 * Computes support point of obj1 and obj2 in direction dir.
 * Support point is returned via supp.
 */
void __ccdSupport(
    const void* obj1,
    const void* obj2,
    const ccd_vec3_t* dir,
    const ccd_t* ccd,
    ccd_support_t* supp);

/**** INLINES ****/
inline void ccdSupportCopy(ccd_support_t* d, const ccd_support_t* s)
{
  *d = *s;
}

} // namespace dart::collision::hit::detail

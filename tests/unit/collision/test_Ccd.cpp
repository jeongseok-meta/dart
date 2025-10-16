/*
 * Copyright (c) 2024, DART Development Team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <gtest/gtest.h>

#include "dart/collision/hit/ccd/ccd.h"
#include "dart/collision/hit/ccd/vec3.h"

//==============================================================================
TEST(CcdTest, Vec3BasicOperations)
{
  dart_ccd_vec3_t v1, v2, v3;

  // Test dartCcdVec3Set
  dartCcdVec3Set(&v1, 1.0, 2.0, 3.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&v1), 1.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&v1), 2.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&v1), 3.0);

  // Test dartCcdVec3Copy
  dartCcdVec3Copy(&v2, &v1);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&v2), 1.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&v2), 2.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&v2), 3.0);

  // Test dartCcdVec3Eq
  EXPECT_TRUE(dartCcdVec3Eq(&v1, &v2));

  // Test dartCcdVec3Sub2
  dartCcdVec3Set(&v2, 4.0, 5.0, 6.0);
  dartCcdVec3Sub2(&v3, &v2, &v1);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&v3), 3.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&v3), 3.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&v3), 3.0);

  // Test dartCcdVec3Add
  dartCcdVec3Add(&v1, &v2);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&v1), 5.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&v1), 7.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&v1), 9.0);
}

//==============================================================================
TEST(CcdTest, Vec3ScalarOperations)
{
  dart_ccd_vec3_t v;

  // Test dartCcdVec3Scale
  dartCcdVec3Set(&v, 1.0, 2.0, 3.0);
  dartCcdVec3Scale(&v, 2.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&v), 2.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&v), 4.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&v), 6.0);

  // Test dartCcdVec3Len2 (squared length)
  dart_ccd_real_t len2 = dartCcdVec3Len2(&v);
  EXPECT_DOUBLE_EQ(len2, 4.0 + 16.0 + 36.0); // 2^2 + 4^2 + 6^2

  // Test dartCcdVec3Normalize
  dartCcdVec3Set(&v, 3.0, 0.0, 4.0); // Length = 5
  dartCcdVec3Normalize(&v);
  EXPECT_NEAR(dartCcdVec3X(&v), 0.6, 1e-10);
  EXPECT_NEAR(dartCcdVec3Y(&v), 0.0, 1e-10);
  EXPECT_NEAR(dartCcdVec3Z(&v), 0.8, 1e-10);
  EXPECT_NEAR(dartCcdVec3Len2(&v), 1.0, 1e-10); // Should be unit length
}

//==============================================================================
TEST(CcdTest, Vec3ProductOperations)
{
  dart_ccd_vec3_t v1, v2, cross;

  // Test dartCcdVec3Dot
  dartCcdVec3Set(&v1, 1.0, 2.0, 3.0);
  dartCcdVec3Set(&v2, 4.0, 5.0, 6.0);
  dart_ccd_real_t dot = dartCcdVec3Dot(&v1, &v2);
  EXPECT_DOUBLE_EQ(dot, 4.0 + 10.0 + 18.0); // 1*4 + 2*5 + 3*6

  // Test dartCcdVec3Cross
  dartCcdVec3Set(&v1, 1.0, 0.0, 0.0);
  dartCcdVec3Set(&v2, 0.0, 1.0, 0.0);
  dartCcdVec3Cross(&cross, &v1, &v2);
  EXPECT_DOUBLE_EQ(dartCcdVec3X(&cross), 0.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Y(&cross), 0.0);
  EXPECT_DOUBLE_EQ(dartCcdVec3Z(&cross), 1.0); // X cross Y = Z
}

//==============================================================================
TEST(CcdTest, Vec3DistanceOperations)
{
  dart_ccd_vec3_t v1, v2;

  // Test dartCcdVec3Dist2 (squared distance)
  dartCcdVec3Set(&v1, 0.0, 0.0, 0.0);
  dartCcdVec3Set(&v2, 3.0, 4.0, 0.0);
  dart_ccd_real_t dist2 = dartCcdVec3Dist2(&v1, &v2);
  EXPECT_DOUBLE_EQ(dist2, 25.0); // 3^2 + 4^2 = 25
}

//==============================================================================
TEST(CcdTest, CcdInitialization)
{
  dart_ccd_t ccd;

  // Test DART_CCD_INIT macro
  DART_CCD_INIT(&ccd);

  EXPECT_NE(ccd.first_dir, nullptr);
  EXPECT_EQ(ccd.support1, nullptr);
  EXPECT_EQ(ccd.support2, nullptr);
  EXPECT_EQ(ccd.center1, nullptr);
  EXPECT_EQ(ccd.center2, nullptr);
  EXPECT_GT(ccd.max_iterations, 0u);
  EXPECT_GT(ccd.epa_tolerance, 0.0);
  EXPECT_GT(ccd.mpr_tolerance, 0.0);
  EXPECT_GT(ccd.dist_tolerance, 0.0);
}

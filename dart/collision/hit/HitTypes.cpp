/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/main/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include <dart/collision/hit/HitTypes.hpp>

namespace dart::collision {

//==============================================================================
Eigen::Vector3d HitTypes::convertVector3(
    const dart::collision::hit::Vector3<double>& vec)
{
  return Eigen::Vector3d(vec[0], vec[1], vec[2]);
}

//==============================================================================
dart::collision::hit::Matrix3<double> HitTypes::convertMatrix3x3(
    const Eigen::Matrix3d& R)
{
  dart::collision::hit::Matrix3<double> result;
  result << R(0, 0), R(0, 1), R(0, 2), R(1, 0), R(1, 1), R(1, 2), R(2, 0),
      R(2, 1), R(2, 2);
  return result;
}

//==============================================================================
dart::collision::hit::Transform3<double> HitTypes::convertTransform(
    const Eigen::Isometry3d& T)
{
  dart::collision::hit::Transform3<double> result;
  result.translation() = dart::collision::hit::Vector3<double>(
      T.translation()[0], T.translation()[1], T.translation()[2]);
  result.linear() = convertMatrix3x3(T.linear());
  return result;
}

} // namespace dart::collision

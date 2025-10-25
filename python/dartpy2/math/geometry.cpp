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

#include <dart/math/Geometry.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/eigen/sparse.h>

namespace nb = nanobind;

// Macros for Euler angle conversions
#define DARTPY2_DEFINE_EULER_TO_MATRIX(order)                                  \
  m.def(                                                                       \
      "euler" #order "ToMatrix",                                               \
      [](const Eigen::Vector3d& angle) -> Eigen::Matrix3d {                    \
        return dart::math::euler##order##ToMatrix(angle);                      \
      },                                                                       \
      nb::arg("angle"));

#define DARTPY2_DEFINE_MATRIX_TO_EULER(order)                                  \
  m.def(                                                                       \
      "matrixToEuler" #order,                                                  \
      [](const Eigen::Matrix3d& R) -> Eigen::Vector3d {                        \
        return dart::math::matrixToEuler##order(R);                            \
      },                                                                       \
      nb::arg("R"));

namespace dart {
namespace python {

void Geometry(nb::module_& m)
{
  // Euler angle conversions
  DARTPY2_DEFINE_EULER_TO_MATRIX(XYX);
  DARTPY2_DEFINE_EULER_TO_MATRIX(XYZ);
  DARTPY2_DEFINE_EULER_TO_MATRIX(XZX);
  DARTPY2_DEFINE_EULER_TO_MATRIX(XZY);
  DARTPY2_DEFINE_EULER_TO_MATRIX(YXY);
  DARTPY2_DEFINE_EULER_TO_MATRIX(YXZ);
  DARTPY2_DEFINE_EULER_TO_MATRIX(YZX);
  DARTPY2_DEFINE_EULER_TO_MATRIX(YZY);
  DARTPY2_DEFINE_EULER_TO_MATRIX(ZXY);
  DARTPY2_DEFINE_EULER_TO_MATRIX(ZYX);
  DARTPY2_DEFINE_EULER_TO_MATRIX(ZXZ);
  DARTPY2_DEFINE_EULER_TO_MATRIX(ZYZ);

  DARTPY2_DEFINE_MATRIX_TO_EULER(XYX);
  DARTPY2_DEFINE_MATRIX_TO_EULER(XYZ);
  DARTPY2_DEFINE_MATRIX_TO_EULER(XZY);
  DARTPY2_DEFINE_MATRIX_TO_EULER(YXZ);
  DARTPY2_DEFINE_MATRIX_TO_EULER(YZX);
  DARTPY2_DEFINE_MATRIX_TO_EULER(ZXY);
  DARTPY2_DEFINE_MATRIX_TO_EULER(ZYX);

  // Exponential map functions
  m.def(
      "expMap",
      [](const Eigen::Vector6d& S) -> Eigen::Isometry3d {
        return math::expMap(S);
      },
      nb::arg("S"));

  m.def(
      "exp_map",
      [](const Eigen::Vector6d& S) -> Eigen::Isometry3d {
        return math::expMap(S);
      },
      nb::arg("S"));

  m.def(
      "expMapJac",
      [](const Eigen::Vector3d& expmap) -> Eigen::Matrix3d {
        return math::expMapJac(expmap);
      },
      nb::arg("expmap"));

  m.def(
      "exp_map_jac",
      [](const Eigen::Vector3d& expmap) -> Eigen::Matrix3d {
        return math::expMapJac(expmap);
      },
      nb::arg("expmap"));

  m.def(
      "expMapRot",
      [](const Eigen::Vector3d& expmap) -> Eigen::Matrix3d {
        return math::expMapRot(expmap);
      },
      nb::arg("expmap"));

  m.def(
      "exp_map_rot",
      [](const Eigen::Vector3d& expmap) -> Eigen::Matrix3d {
        return math::expMapRot(expmap);
      },
      nb::arg("expmap"));

  m.def(
      "expToQuat",
      [](const Eigen::Vector3d& v) -> Eigen::Quaterniond {
        return math::expToQuat(v);
      },
      nb::arg("v"));

  m.def(
      "exp_to_quat",
      [](const Eigen::Vector3d& v) -> Eigen::Quaterniond {
        return math::expToQuat(v);
      },
      nb::arg("v"));

  m.def(
      "quatToExp",
      [](const Eigen::Quaterniond& q) -> Eigen::Vector3d {
        return math::quatToExp(q);
      },
      nb::arg("q"));

  m.def(
      "quat_to_exp",
      [](const Eigen::Quaterniond& q) -> Eigen::Vector3d {
        return math::quatToExp(q);
      },
      nb::arg("q"));

  m.def(
      "expAngular",
      [](const Eigen::Vector3d& s) -> Eigen::Isometry3d {
        return math::expAngular(s);
      },
      nb::arg("s"));

  m.def(
      "exp_angular",
      [](const Eigen::Vector3d& s) -> Eigen::Isometry3d {
        return math::expAngular(s);
      },
      nb::arg("s"));

  // Verification functions
  m.def(
      "verifyRotation",
      [](const Eigen::Matrix3d& R) -> bool {
        return math::verifyRotation(R);
      },
      nb::arg("R"));

  m.def(
      "verify_rotation",
      [](const Eigen::Matrix3d& R) -> bool {
        return math::verifyRotation(R);
      },
      nb::arg("R"));

  m.def(
      "verifyTransform",
      [](const Eigen::Isometry3d& T) -> bool {
        return math::verifyTransform(T);
      },
      nb::arg("T"));

  m.def(
      "verify_transform",
      [](const Eigen::Isometry3d& T) -> bool {
        return math::verifyTransform(T);
      },
      nb::arg("T"));
}

} // namespace python
} // namespace dart

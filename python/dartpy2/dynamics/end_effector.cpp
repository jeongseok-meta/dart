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

#include <dart/dynamics/EndEffector.hpp>
#include <dart/dynamics/InverseKinematics.hpp>

#include <Eigen/Geometry>
#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dart::python {

void defEndEffector(nb::module_& m)
{
  // EndEffector is managed by BodyNode via raw pointers
  nb::class_<dynamics::EndEffector>(m, "EndEffector")
      .def("__repr__",
          [](const dynamics::EndEffector& self) {
            return fmt::format(
                "EndEffector(name='{}')",
                self.getName());
          })
      .def("get_name", &dynamics::EndEffector::getName,
          "Get the name of this end effector")
      .def("set_name", &dynamics::EndEffector::setName,
          nb::arg("name"),
          "Set the name of this end effector")
      .def("get_ik", [](dynamics::EndEffector& self, bool create_if_null) -> dynamics::InverseKinematics* {
        auto ik_ptr = self.getIK(create_if_null);
        return ik_ptr.get();
      }, nb::arg("create_if_null") = false,
      nb::rv_policy::reference_internal,
      "Get or create the IK module for this end effector")
      .def("set_default_relative_transform",
          [](dynamics::EndEffector& self, const Eigen::Isometry3d& transform, const dynamics::Frame* relative_to) {
            self.setDefaultRelativeTransform(transform, relative_to);
          },
          nb::arg("transform"),
          nb::arg("relative_to"),
          "Set the default relative transform for this end effector")
      .def("set_default_relative_transform",
          [](dynamics::EndEffector& self, const Eigen::Isometry3d& transform, bool with_respect_to_parent_frame) {
            self.setDefaultRelativeTransform(transform, with_respect_to_parent_frame);
          },
          nb::arg("transform"),
          nb::arg("with_respect_to_parent_frame"),
          "Set the default relative transform with respect to parent frame")
      .def("set_relative_transform",
          [](dynamics::EndEffector& self, const Eigen::Isometry3d& transform) {
            self.setRelativeTransform(transform);
          },
          nb::arg("transform"),
          "Set the relative transform for this end effector")
      .def("get_transform",
          [](const dynamics::EndEffector& self) -> Eigen::Isometry3d {
            return self.getTransform();
          },
          "Get the transform of this end effector")
      .def("get_world_transform",
          [](const dynamics::EndEffector& self) -> Eigen::Isometry3d {
            return self.getWorldTransform();
          },
          "Get the world transform of this end effector")
      .def("reset_relative_transform",
          &dynamics::EndEffector::resetRelativeTransform,
          "Reset the relative transform to the default")
      .def("get_support",
          [](dynamics::EndEffector& self, bool create_if_null) {
            return self.getSupport(create_if_null);
          },
          nb::arg("create_if_null") = false,
          nb::rv_policy::reference_internal,
          "Get or create the Support module for this end effector");
}

} // namespace dart::python

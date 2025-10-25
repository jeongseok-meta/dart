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

#include <dart/dynamics/DegreeOfFreedom.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defDegreeOfFreedom(nb::module_& m)
{
  // Note: DegreeOfFreedom is managed via raw pointers, not shared_ptr
  // The Skeleton owns the DOFs through its joints
  nb::class_<dynamics::DegreeOfFreedom>(m, "DegreeOfFreedom")
      .def("__repr__",
          [](const dynamics::DegreeOfFreedom& self) {
            return fmt::format(
                "DegreeOfFreedom(name='{}', position={:.3f})",
                self.getName(),
                self.getPosition());
          })
      .def("get_name", &dynamics::DegreeOfFreedom::getName,
          "Get the name of this DOF")
      .def("get_index_in_skeleton",
          &dynamics::DegreeOfFreedom::getIndexInSkeleton,
          "Get the index of this DOF in the skeleton")
      .def("get_index_in_joint", &dynamics::DegreeOfFreedom::getIndexInJoint,
          "Get the index of this DOF within its joint")
      .def("set_position", &dynamics::DegreeOfFreedom::setPosition,
          nb::arg("position"),
          "Set the position (configuration) of this DOF")
      .def("get_position", &dynamics::DegreeOfFreedom::getPosition,
          "Get the position (configuration) of this DOF")
      .def("set_velocity", &dynamics::DegreeOfFreedom::setVelocity,
          nb::arg("velocity"),
          "Set the velocity of this DOF")
      .def("get_velocity", &dynamics::DegreeOfFreedom::getVelocity,
          "Get the velocity of this DOF")
      .def("set_acceleration", &dynamics::DegreeOfFreedom::setAcceleration,
          nb::arg("acceleration"),
          "Set the acceleration of this DOF")
      .def("get_acceleration", &dynamics::DegreeOfFreedom::getAcceleration,
          "Get the acceleration of this DOF")
      .def("set_force", &dynamics::DegreeOfFreedom::setForce,
          nb::arg("force"),
          "Set the generalized force for this DOF")
      .def("get_force", &dynamics::DegreeOfFreedom::getForce,
          "Get the generalized force for this DOF")
      .def("set_position_limits",
          nb::overload_cast<double, double>(
              &dynamics::DegreeOfFreedom::setPositionLimits),
          nb::arg("lower_limit"),
          nb::arg("upper_limit"),
          "Set the position limits for this DOF")
      .def("get_position_limits",
          &dynamics::DegreeOfFreedom::getPositionLimits,
          "Get the position limits for this DOF as (lower, upper) tuple")
      .def("get_position_lower_limit",
          &dynamics::DegreeOfFreedom::getPositionLowerLimit,
          "Get the lower position limit")
      .def("get_position_upper_limit",
          &dynamics::DegreeOfFreedom::getPositionUpperLimit,
          "Get the upper position limit")
      .def("has_position_limit",
          &dynamics::DegreeOfFreedom::hasPositionLimit,
          "Check if this DOF has position limits")
      .def("reset_position", &dynamics::DegreeOfFreedom::resetPosition,
          "Reset position to initial value");
}

} // namespace dart::python

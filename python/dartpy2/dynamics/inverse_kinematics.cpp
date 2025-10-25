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

#include <dart/dynamics/InverseKinematics.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dart::python {

void defInverseKinematics(nb::module_& m)
{
  // InverseKinematics is managed via shared_ptr by EndEffector
  nb::class_<dynamics::InverseKinematics>(m, "InverseKinematics")
      .def("__repr__",
          [](const dynamics::InverseKinematics&) {
            return fmt::format("InverseKinematics()");
          })
      .def("solve_and_apply",
          [](dynamics::InverseKinematics& self, bool allow_incomplete_result) {
            return self.solveAndApply(allow_incomplete_result);
          },
          nb::arg("allow_incomplete_result") = true,
          "Solve the IK problem and apply the solution")
      .def("clear_caches", &dynamics::InverseKinematics::clearCaches,
          "Clear the caches")
      .def("set_hierarchy_level",
          &dynamics::InverseKinematics::setHierarchyLevel,
          nb::arg("level"),
          "Set the hierarchy level of this IK module")
      .def("get_hierarchy_level",
          &dynamics::InverseKinematics::getHierarchyLevel,
          "Get the hierarchy level of this IK module")
      .def("use_chain",
          &dynamics::InverseKinematics::useChain,
          "Restrict this IK to the chain leading to its end effector")
      .def("use_whole_body",
          &dynamics::InverseKinematics::useWholeBody,
          "Allow this IK to use the whole body")
      .def("set_active",
          nb::overload_cast<bool>(&dynamics::InverseKinematics::setActive),
          nb::arg("active"),
          "Set whether this IK module is active")
      .def("is_active",
          &dynamics::InverseKinematics::isActive,
          "Check if this IK module is active")
      .def("set_target",
          [](dynamics::InverseKinematics& self,
             const std::shared_ptr<dynamics::SimpleFrame>& target) {
            self.setTarget(target);
          },
          nb::arg("target"),
          "Set the target frame for this IK module")
      .def("get_target",
          [](dynamics::InverseKinematics& self) {
            return self.getTarget();
          },
          "Get the target frame for this IK module")
      .def("get_error_method",
          [](dynamics::InverseKinematics& self) -> dynamics::InverseKinematics::ErrorMethod& {
            return self.getErrorMethod();
          },
          nb::rv_policy::reference_internal,
          "Get the ErrorMethod for this IK module")
      .def("get_gradient_method",
          [](dynamics::InverseKinematics& self) -> dynamics::InverseKinematics::GradientMethod& {
            return self.getGradientMethod();
          },
          nb::rv_policy::reference_internal,
          "Get the GradientMethod for this IK module")
      .def("get_problem",
          [](dynamics::InverseKinematics& self) {
            return self.getProblem();
          },
          "Get the optimization Problem for this IK module")
      .def("set_objective",
          [](dynamics::InverseKinematics& self,
             const std::shared_ptr<optimizer::Function>& objective) {
            self.setObjective(objective);
          },
          nb::arg("objective"),
          "Set the objective function for this IK module")
      .def("get_solver",
          [](dynamics::InverseKinematics& self) {
            return self.getSolver();
          },
          "Get the optimization Solver for this IK module");

  // ErrorMethod nested class
  nb::class_<dynamics::InverseKinematics::ErrorMethod>(m, "IKErrorMethod")
      .def("set_bounds",
          [](dynamics::InverseKinematics::ErrorMethod& self) {
            self.setBounds();
          },
          "Reset to standard default bounds")
      .def("get_bounds",
          [](const dynamics::InverseKinematics::ErrorMethod& self) {
            return self.getBounds();
          },
          "Get the error bounds as a pair of (linear_bounds, angular_bounds)")
      .def("set_linear_bounds",
          [](dynamics::InverseKinematics::ErrorMethod& self,
             const Eigen::Vector3d& lower,
             const Eigen::Vector3d& upper) {
            self.setLinearBounds(lower, upper);
          },
          nb::arg("lower"),
          nb::arg("upper"),
          "Set the linear error bounds")
      .def("get_linear_bounds",
          [](const dynamics::InverseKinematics::ErrorMethod& self) {
            return self.getLinearBounds();
          },
          "Get the linear error bounds as a pair of (lower, upper)")
      .def("set_angular_bounds",
          [](dynamics::InverseKinematics::ErrorMethod& self,
             const Eigen::Vector3d& lower,
             const Eigen::Vector3d& upper) {
            self.setAngularBounds(lower, upper);
          },
          nb::arg("lower"),
          nb::arg("upper"),
          "Set the angular error bounds")
      .def("get_angular_bounds",
          [](const dynamics::InverseKinematics::ErrorMethod& self) {
            return self.getAngularBounds();
          },
          "Get the angular error bounds as a pair of (lower, upper)");

  // GradientMethod nested class
  nb::class_<dynamics::InverseKinematics::GradientMethod>(m, "IKGradientMethod")
      .def("set_component_weights",
          [](dynamics::InverseKinematics::GradientMethod& self,
             const Eigen::VectorXd& weights) {
            self.setComponentWeights(weights);
          },
          nb::arg("weights"),
          "Set the component weights for the gradient");
}

} // namespace dart::python

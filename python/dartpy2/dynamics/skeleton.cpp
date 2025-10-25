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

#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/DegreeOfFreedom.hpp>
#include <dart/dynamics/Skeleton.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defSkeleton(nb::module_& m)
{
  // Note: Skeleton uses Skeleton.create() factory pattern rather than Skeleton()
  // constructor. This is due to DART's C++ architecture where Skeleton must be
  // managed via shared_ptr and has private constructors.
  //
  // While nanobind supports shared_ptr through type casters, creating Pythonic
  // constructors (__init__ or __new__) for factory-created shared_ptr types is
  // complex and not well-documented. The .create() approach is explicit and works
  // reliably across all nanobind versions.
  //
  // This is less Pythonic but matches DART's C++ API and is clear about the
  // factory pattern being used.
  nb::class_<dynamics::Skeleton>(m, "Skeleton")
      .def_static(
          "create",
          []() -> std::shared_ptr<dynamics::Skeleton> {
            return dynamics::Skeleton::create();
          },
          "Create an empty Skeleton")
      .def_static(
          "create",
          [](const std::string& name) -> std::shared_ptr<dynamics::Skeleton> {
            return dynamics::Skeleton::create(name);
          },
          nb::arg("name"),
          "Create a Skeleton with a name")
      .def("__repr__", [](const dynamics::Skeleton& self) {
        return fmt::format(
            "Skeleton(name='{}', num_body_nodes={})",
            self.getName(),
            self.getNumBodyNodes());
      })
      .def("get_name", &dynamics::Skeleton::getName)
      .def("set_name", &dynamics::Skeleton::setName, nb::arg("name"))
      .def("get_num_body_nodes", &dynamics::Skeleton::getNumBodyNodes)
      .def("get_num_dofs", &dynamics::Skeleton::getNumDofs)
      .def("get_positions",
          [](const dynamics::Skeleton& self) { return self.getPositions(); })
      .def("set_positions",
          [](dynamics::Skeleton& self, const Eigen::VectorXd& positions) {
            self.setPositions(positions);
          },
          nb::arg("positions"))
        .def("get_dof",
            nb::overload_cast<std::size_t>(&dynamics::Skeleton::getDof),
            nb::arg("index"),
            nb::rv_policy::reference_internal,
            "Get DOF by index")
        .def("get_dof",
            nb::overload_cast<const std::string&>(&dynamics::Skeleton::getDof),
            nb::arg("name"),
            nb::rv_policy::reference_internal,
            "Get DOF by name")
        .def("get_body_node",
            nb::overload_cast<std::size_t>(&dynamics::Skeleton::getBodyNode),
            nb::arg("index"),
            nb::rv_policy::reference_internal,
            "Get BodyNode by index")
        .def("get_body_node",
            nb::overload_cast<const std::string&>(&dynamics::Skeleton::getBodyNode),
            nb::arg("name"),
            nb::rv_policy::reference_internal,
            "Get BodyNode by name")
          .def("get_end_effector",
              nb::overload_cast<std::size_t>(&dynamics::Skeleton::getEndEffector),
              nb::arg("index"),
              nb::rv_policy::reference_internal,
              "Get EndEffector by index")
          .def("get_end_effector",
              nb::overload_cast<const std::string&>(&dynamics::Skeleton::getEndEffector),
              nb::arg("name"),
              nb::rv_policy::reference_internal,
              "Get EndEffector by name")
          .def("get_num_end_effectors",
              [](const dynamics::Skeleton& self) { return self.getNumEndEffectors(); },
              "Get the number of EndEffectors in this Skeleton")
          .def("get_joint",
              nb::overload_cast<std::size_t>(&dynamics::Skeleton::getJoint),
              nb::arg("index"),
              nb::rv_policy::reference_internal,
              "Get Joint by index")
          .def("get_joint",
              nb::overload_cast<const std::string&>(&dynamics::Skeleton::getJoint),
              nb::arg("name"),
              nb::rv_policy::reference_internal,
              "Get Joint by name")
          .def("get_ik",
              nb::overload_cast<bool>(&dynamics::Skeleton::getIK),
              nb::arg("create_if_null") = false,
              "Get the whole-body IK module for this skeleton");
}

} // namespace dart::python

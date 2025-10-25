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
#include <dart/dynamics/EndEffector.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dart::python {

void defBodyNode(nb::module_& m)
{
  // BodyNode is managed by Skeleton via raw pointers
  nb::class_<dynamics::BodyNode>(m, "BodyNode")
      .def("__repr__",
          [](const dynamics::BodyNode& self) {
            return fmt::format(
                "BodyNode(name='{}')",
                self.getName());
          })
      .def("get_name", &dynamics::BodyNode::getName,
          "Get the name of this body node")
      .def("set_name", &dynamics::BodyNode::setName,
          nb::arg("name"),
          "Set the name of this body node")
        .def("get_num_child_body_nodes",
            &dynamics::BodyNode::getNumChildBodyNodes,
            "Get the number of child body nodes")
        .def("create_end_effector",
            [](dynamics::BodyNode& self, const std::string& name) {
              return self.createEndEffector(name);
            },
            nb::arg("name") = "EndEffector",
            nb::rv_policy::reference_internal,
            "Create an end effector for this body node")
        .def("get_world_transform",
            [](const dynamics::BodyNode& self) {
              return self.getWorldTransform();
            },
            "Get the world transform of this body node");
}

} // namespace dart::python

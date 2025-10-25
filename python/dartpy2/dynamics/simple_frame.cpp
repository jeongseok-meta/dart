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

#include <dart/dynamics/SimpleFrame.hpp>
#include <dart/dynamics/Frame.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defSimpleFrame(nb::module_& m)
{
  // SimpleFrame inherits from Detachable and ShapeFrame in C++
  // ShapeFrame inherits from Frame, so SimpleFrame has all Frame methods
  // Nanobind automatically handles shared_ptr - don't specify it as template parameter
  nb::class_<dynamics::SimpleFrame>(m, "SimpleFrame")
      .def(nb::init<>(),
          "Create a SimpleFrame with default constructor")
      .def(nb::init<dynamics::Frame*>(),
          nb::arg("ref_frame"),
          "Create a SimpleFrame with a parent frame")
      .def(nb::init<dynamics::Frame*, const std::string&>(),
          nb::arg("ref_frame"),
          nb::arg("name"),
          "Create a SimpleFrame with a parent frame and name")
      .def(nb::init<dynamics::Frame*, const std::string&, const Eigen::Isometry3d&>(),
          nb::arg("ref_frame"),
          nb::arg("name"),
          nb::arg("relative_transform"),
          "Create a SimpleFrame with a parent frame, name, and relative transform")
      // Static factory methods (wrapping std::make_shared)
      .def_static("create",
          []() { return std::make_shared<dynamics::SimpleFrame>(); },
          "Create a shared_ptr to SimpleFrame with default constructor")
      .def_static("create",
          [](dynamics::Frame* refFrame) {
            return std::make_shared<dynamics::SimpleFrame>(refFrame);
          },
          nb::arg("ref_frame").none(),
          "Create a shared_ptr to SimpleFrame with parent frame")
      .def_static("create",
          [](dynamics::Frame* refFrame, const std::string& name) {
            return std::make_shared<dynamics::SimpleFrame>(refFrame, name);
          },
          nb::arg("ref_frame").none(),
          nb::arg("name"),
          "Create a shared_ptr to SimpleFrame with parent frame and name")
      .def_static("create",
          [](dynamics::Frame* refFrame, const std::string& name, const Eigen::Isometry3d& relativeTransform) {
            return std::make_shared<dynamics::SimpleFrame>(refFrame, name, relativeTransform);
          },
          nb::arg("ref_frame").none(),
          nb::arg("name"),
          nb::arg("relative_transform"),
          "Create a shared_ptr to SimpleFrame with parent frame, name, and relative transform")
      .def("__repr__",
          [](const dynamics::SimpleFrame& self) {
            return fmt::format("SimpleFrame(name='{}')", self.getName());
          })
      .def("get_name", &dynamics::SimpleFrame::getName,
          "Get the name of this frame")
      .def("set_name", &dynamics::SimpleFrame::setName,
          nb::arg("name"),
          "Set the name of this frame")
      .def("clone",
          [](const dynamics::SimpleFrame& self) {
            return self.clone();
          },
          "Clone this SimpleFrame")
      .def("clone",
          [](const dynamics::SimpleFrame& self, dynamics::Frame* refFrame) {
            return self.clone(refFrame);
          },
          nb::arg("ref_frame"),
          "Clone this SimpleFrame with a new parent frame")
      .def("spawn_child_simple_frame",
          [](dynamics::SimpleFrame& self) {
            return self.spawnChildSimpleFrame();
          },
          "Spawn a child SimpleFrame")
      .def("spawn_child_simple_frame",
          [](dynamics::SimpleFrame& self, const std::string& name) {
            return self.spawnChildSimpleFrame(name);
          },
          nb::arg("name"),
          "Spawn a child SimpleFrame with a name")
      .def("spawn_child_simple_frame",
          [](dynamics::SimpleFrame& self, const std::string& name, const Eigen::Isometry3d& relativeTransform) {
            return self.spawnChildSimpleFrame(name, relativeTransform);
          },
          nb::arg("name"),
          nb::arg("relative_transform"),
          "Spawn a child SimpleFrame with a name and relative transform")
      .def("set_relative_transform",
          &dynamics::SimpleFrame::setRelativeTransform,
          nb::arg("new_relative_transform"),
          "Set the relative transform of this frame")
      .def("set_relative_translation",
          &dynamics::SimpleFrame::setRelativeTranslation,
          nb::arg("new_translation"),
          "Set the relative translation of this frame")
      .def("set_relative_rotation",
          &dynamics::SimpleFrame::setRelativeRotation,
          nb::arg("new_rotation"),
          "Set the relative rotation of this frame")
      .def("set_transform",
          [](dynamics::SimpleFrame& self, const Eigen::Isometry3d& newTransform) {
            self.setTransform(newTransform);
          },
          nb::arg("new_transform"),
          "Set the transform of this frame")
      .def("set_translation",
          [](dynamics::SimpleFrame& self, const Eigen::Vector3d& newTranslation) {
            self.setTranslation(newTranslation);
          },
          nb::arg("new_translation"),
          "Set the translation of this frame")
      .def("set_rotation",
          [](dynamics::SimpleFrame& self, const Eigen::Matrix3d& newRotation) {
            self.setRotation(newRotation);
          },
          nb::arg("new_rotation"),
          "Set the rotation of this frame")
      // Frame methods - SimpleFrame inherits from Frame through ShapeFrame
      // We need to explicitly bind these since nanobind doesn't automatically expose inherited methods
      .def("get_transform",
          [](const dynamics::SimpleFrame& self) {
            return self.getTransform();
          },
          "Get the transform of this frame")
      .def("get_relative_transform",
          [](const dynamics::SimpleFrame& self) {
            return self.getRelativeTransform();
          },
          "Get the relative transform of this frame")
        .def("get_world_transform",
            [](const dynamics::SimpleFrame& self) {
              return self.getWorldTransform();
            },
            "Get the world transform of this frame")
        // ShapeFrame methods (SimpleFrame inherits from ShapeFrame)
        .def("set_shape",
            [](dynamics::SimpleFrame& self, const std::shared_ptr<dynamics::Shape>& shape) {
              self.setShape(shape);
            },
            nb::arg("shape"),
            "Set the shape of this frame")
        .def("get_shape",
            [](const dynamics::SimpleFrame& self) {
              return self.getShape();
            },
            nb::rv_policy::reference_internal,
            "Get the shape of this frame");
}

} // namespace dart::python

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

#include <dart/gui/osg/all.hpp>
#include <dart/gui/osg/detail/CameraModeCallback.hpp>
#include <dart/dynamics/BodyNode.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defViewer(nb::module_& m)
{
  // Viewer - minimal API following binding philosophy
  // Essential methods for creating GUI windows and rendering worlds
  nb::class_<gui::osg::Viewer>(m, "Viewer")
      .def(nb::init<>(), "Create a default Viewer")
      .def(
          "add_world_node",
          [](gui::osg::Viewer& self, gui::osg::WorldNode* node) {
            self.addWorldNode(node);
          },
          nb::arg("world_node"),
          "Add a WorldNode to the viewer")
      .def(
          "set_up_view_in_window",
          [](gui::osg::Viewer& self, int x, int y, int width, int height) {
            self.setUpViewInWindow(x, y, width, height);
          },
          nb::arg("x"),
          nb::arg("y"),
          nb::arg("width"),
          nb::arg("height"),
          "Set up the view in a window at (x,y) with given width and height")
      .def(
          "run",
          [](gui::osg::Viewer& self) -> int {
            return self.run();
          },
          "Run the viewer main loop. Blocks until window is closed. Returns 0 on success")
      .def(
          "frame",
          [](gui::osg::Viewer& self) {
            self.frame();
          },
          "Render a single frame (non-blocking)")
      .def(
          "frame",
          [](gui::osg::Viewer& self, double simulation_time) {
            self.frame(simulation_time);
          },
          nb::arg("simulation_time"),
          "Render a single frame with simulation time (non-blocking)")
      .def(
          "allow_simulation",
          [](gui::osg::Viewer& self, bool allow) {
            self.allowSimulation(allow);
          },
          nb::arg("allow"),
          "Allow or prevent simulation from running")
      .def(
          "enable_drag_and_drop",
          [](gui::osg::Viewer& self, gui::osg::InteractiveFrame* frame) {
            return self.enableDragAndDrop(frame);
          },
          nb::arg("frame"),
          "Enable drag-and-drop manipulation for an InteractiveFrame")
      .def(
          "enable_drag_and_drop",
          [](gui::osg::Viewer& self, dynamics::BodyNode* body_node, bool use_extrinsic, bool use_parent_frame) {
            return self.enableDragAndDrop(body_node, use_extrinsic, use_parent_frame);
          },
          nb::arg("body_node"),
          nb::arg("use_extrinsic") = true,
          nb::arg("use_parent_frame") = false,
          "Enable drag-and-drop manipulation for a BodyNode")
        .def(
            "set_camera_home_position",
            [](gui::osg::Viewer& self,
               const Eigen::Vector3d& eye,
               const Eigen::Vector3d& center,
               const Eigen::Vector3d& up) {
              self.getCameraManipulator()->setHomePosition(
                  gui::osg::eigToOsgVec3(eye),
                  gui::osg::eigToOsgVec3(center),
                  gui::osg::eigToOsgVec3(up));
              self.setCameraManipulator(self.getCameraManipulator());
            },
            nb::arg("eye"),
            nb::arg("center"),
            nb::arg("up"),
            "Set the camera home position (eye, center, up vectors)")
        .def(
            "add_attachment",
            [](gui::osg::Viewer& self, gui::osg::ViewerAttachment* attachment) {
              self.addAttachment(attachment);
            },
            nb::arg("attachment"),
            "Add an attachment (e.g., SupportPolygonVisual) to the viewer");
}

} // namespace dart::python

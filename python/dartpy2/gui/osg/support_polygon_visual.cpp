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

#include <dart/gui/osg/SupportPolygonVisual.hpp>
#include <dart/dynamics/Skeleton.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

namespace nb = nanobind;

namespace dart::python {

void defSupportPolygonVisual(nb::module_& m)
{
  // ViewerAttachment base class
  nb::class_<gui::osg::ViewerAttachment>(m, "ViewerAttachment")
      .def("refresh", &gui::osg::ViewerAttachment::refresh,
          "Update the attachment");

  // SupportPolygonVisual - visualizes support polygon and center of mass
  nb::class_<gui::osg::SupportPolygonVisual, gui::osg::ViewerAttachment>(
      m, "SupportPolygonVisual")
      .def(nb::init<const dynamics::SkeletonPtr&, double>(),
          nb::arg("skeleton") = nullptr,
          nb::arg("elevation") = 0.02,
          "Create a SupportPolygonVisual for an entire Skeleton")
      .def(nb::init<const dynamics::SkeletonPtr&, std::size_t, double>(),
          nb::arg("skeleton"),
          nb::arg("tree_index"),
          nb::arg("elevation") = 0.02,
          "Create a SupportPolygonVisual for a specific tree in a Skeleton")
      .def("set_skeleton",
          &gui::osg::SupportPolygonVisual::setSkeleton,
          nb::arg("skeleton"),
          "Change the Skeleton that is being visualized")
      .def("get_skeleton",
          &gui::osg::SupportPolygonVisual::getSkeleton,
          "Get the Skeleton associated with this visual")
      .def("visualize_whole_skeleton",
          &gui::osg::SupportPolygonVisual::visualizeWholeSkeleton,
          "Visualize the entire Skeleton")
      .def("visualize_tree",
          &gui::osg::SupportPolygonVisual::visualizeTree,
          nb::arg("tree_index"),
          "Visualize a specific tree in the Skeleton")
      .def("set_display_elevation",
          &gui::osg::SupportPolygonVisual::setDisplayElevation,
          nb::arg("elevation"),
          "Change the elevation height at which the polygon is displayed")
      .def("get_display_elevation",
          &gui::osg::SupportPolygonVisual::getDisplayElevation,
          "Get the elevation of display for the support polygon")
      .def("display_polygon",
          &gui::osg::SupportPolygonVisual::displayPolygon,
          nb::arg("display"),
          "Display the support polygon")
      .def("set_polygon_color",
          &gui::osg::SupportPolygonVisual::setPolygonColor,
          nb::arg("color"),
          "Set the color of the support polygon")
      .def("display_centroid",
          &gui::osg::SupportPolygonVisual::displayCentroid,
          nb::arg("display"),
          "Display the centroid")
      .def("set_centroid_radius",
          &gui::osg::SupportPolygonVisual::setCentroidRadius,
          nb::arg("radius"),
          "Set the radius of the centroid visualization")
      .def("display_center_of_mass",
          &gui::osg::SupportPolygonVisual::displayCenterOfMass,
          nb::arg("display"),
          "Display the center of mass")
      .def("set_center_of_mass_radius",
          &gui::osg::SupportPolygonVisual::setCenterOfMassRadius,
          nb::arg("radius"),
          "Set the radius of the center of mass visualization")
      .def("set_valid_com_color",
          &gui::osg::SupportPolygonVisual::setValidCOMColor,
          nb::arg("color"),
          "Set the color for the center of mass when its projection is on the support polygon")
      .def("set_invalid_com_color",
          &gui::osg::SupportPolygonVisual::setInvalidCOMColor,
          nb::arg("color"),
          "Set the color for the center of mass when its projection is NOT on the support polygon");
}

} // namespace dart::python

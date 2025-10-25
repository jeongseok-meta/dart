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
#include <dart/math/Geometry.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/vector.h>

namespace nb = nanobind;

namespace dart::python {

void defSupport(nb::module_& m)
{
  // Support is managed by EndEffector via raw pointers (Aspect pattern)
  nb::class_<dynamics::Support>(m, "Support")
      .def("__repr__",
          [](const dynamics::Support& self) {
            return fmt::format("Support(active={})", self.isActive());
          })
      .def("set_geometry",
          [](dynamics::Support& self,
             const std::vector<Eigen::Vector3d>& geometry) {
            self.setGeometry(geometry);
          },
          nb::arg("geometry"),
          "Set the support geometry points in the EndEffector frame")
      .def("get_geometry",
          [](const dynamics::Support& self) {
            return self.getGeometry();
          },
          "Get the support geometry points")
      .def("setGeometry",
          [](dynamics::Support& self,
             const std::vector<Eigen::Vector3d>& geometry) {
            self.setGeometry(geometry);
          },
          nb::arg("geometry"),
          "Set the support geometry points in the EndEffector frame")
      .def("setActive",
          [](dynamics::Support& self) {
            self.setActive();
          },
          "Set this EndEffector as active for support")
      .def("set_active",
          [](dynamics::Support& self, bool active) {
            self.setActive(active);
          },
          nb::arg("active") = true,
          "Set whether this EndEffector should be used for support (e.g., as a foot)")
      .def("set_inactive",
          [](dynamics::Support& self) {
            self.setActive(false);
          },
          "Disable this EndEffector from being used for support")
      .def("is_active",
          &dynamics::Support::isActive,
          "Check if this EndEffector is currently being used for support");
}

} // namespace dart::python

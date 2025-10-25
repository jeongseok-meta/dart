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

#include "../eigen_python.hpp"

#include <dart/simulation/World.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void World(nb::module_& m)
{
  nb::class_<simulation::World>(m, "World")
      .def(nb::init<>())
      .def(nb::init<const std::string&>(), nb::arg("name") = "")
      .def("__repr__", [](const simulation::World& self) {
        return fmt::format(
            "World(name='{}', num_skeletons={}, time={}s)",
            self.getName(),
            self.getNumSkeletons(),
            self.getTime());
      })
      .def("set_name", &simulation::World::setName, nb::arg("name"))
      .def("get_name", &simulation::World::getName)
      .def("set_gravity",
          [](simulation::World& self, nb::object gravityObj) {
            self.setGravity(toEigenVector3d(gravityObj));
          },
          nb::arg("gravity"))
      .def("get_gravity", &simulation::World::getGravity)
      .def("set_time_step", &simulation::World::setTimeStep, nb::arg("time_step"))
      .def("get_time_step", &simulation::World::getTimeStep)
        .def("get_num_skeletons", &simulation::World::getNumSkeletons)
        .def("add_skeleton",
            &simulation::World::addSkeleton,
            nb::arg("skeleton"),
            "Add a Skeleton to the World")
        .def("reset", &simulation::World::reset)
        .def("step",
            [](simulation::World& self) { self.step(); })
        .def("step",
            [](simulation::World& self, bool reset_command) { self.step(reset_command); },
            nb::arg("reset_command"))
          .def("set_time", &simulation::World::setTime, nb::arg("time"))
          .def("get_time", &simulation::World::getTime)
          .def("get_sim_frames", &simulation::World::getSimFrames)
          .def("add_simple_frame",
              &simulation::World::addSimpleFrame,
              nb::arg("frame"),
              "Add a SimpleFrame to the World for visualization")
          .def("remove_simple_frame",
              &simulation::World::removeSimpleFrame,
              nb::arg("frame"),
              "Remove a SimpleFrame from the World");
}

} // namespace python
} // namespace dart

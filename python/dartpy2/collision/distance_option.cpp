/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void DistanceOption(nb::module_& m)
{
  ::nb::class_<dart::collision::DistanceOption>(m, "DistanceOption")
      .def(::nb::init<>())
      .def(::nb::init<bool>(), ::nb::arg("enableNearestPoints"))
      .def(
          ::nb::init<bool, double>(),
          ::nb::arg("enableNearestPoints"),
          ::nb::arg("distanceLowerBound"))
      .def(
          ::nb::init<
              bool,
              double,
              const std::shared_ptr<dart::collision::DistanceFilter>&>(),
          ::nb::arg("enableNearestPoints"),
          ::nb::arg("distanceLowerBound"),
          ::nb::arg("distanceFilter"))
      .def_rw(
          "enableNearestPoints",
          &dart::collision::DistanceOption::enableNearestPoints)
      .def_rw(
          "distanceLowerBound",
          &dart::collision::DistanceOption::distanceLowerBound)
      .def_rw(
          "distanceFilter", &dart::collision::DistanceOption::distanceFilter);
}

} // namespace python
} // namespace dart

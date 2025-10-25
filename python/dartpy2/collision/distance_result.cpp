/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void DistanceResult(nb::module_& m)
{
  ::nb::class_<dart::collision::DistanceResult>(m, "DistanceResult")
      .def(::nb::init<>())
      .def(
          "clear",
          +[](dart::collision::DistanceResult* self) { self->clear(); })
      .def(
          "found",
          +[](const dart::collision::DistanceResult* self) -> bool {
            return self->found();
          })
      .def(
          "isMinDistanceClamped",
          +[](const dart::collision::DistanceResult* self) -> bool {
            return self->isMinDistanceClamped();
          })
      .def_rw(
          "minDistance", &dart::collision::DistanceResult::minDistance)
      .def_rw(
          "unclampedMinDistance",
          &dart::collision::DistanceResult::unclampedMinDistance)
      .def_rw(
          "shapeFrame1", &dart::collision::DistanceResult::shapeFrame1)
      .def_rw(
          "shapeFrame2", &dart::collision::DistanceResult::shapeFrame2)
      .def_rw(
          "nearestPoint1", &dart::collision::DistanceResult::nearestPoint1)
      .def_rw(
          "nearestPoint2", &dart::collision::DistanceResult::nearestPoint2);
}

} // namespace python
} // namespace dart

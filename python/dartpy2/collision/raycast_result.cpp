/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/vector.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void RaycastResult(nb::module_& m)
{
  ::nb::class_<dart::collision::RayHit>(m, "RayHit")
      .def(::nb::init<>())
      .def_rw(
          "mCollisionObject", &dart::collision::RayHit::mCollisionObject)
      .def_rw("mNormal", &dart::collision::RayHit::mNormal)
      .def_rw("mPoint", &dart::collision::RayHit::mPoint)
      .def_rw("mFraction", &dart::collision::RayHit::mFraction);

  ::nb::class_<dart::collision::RaycastResult>(m, "RaycastResult")
      .def(::nb::init<>())
      .def(
          "clear", +[](dart::collision::RaycastResult* self) { self->clear(); })
      .def(
          "hasHit",
          +[](const dart::collision::RaycastResult* self) -> bool {
            return self->hasHit();
          })
      .def_rw("mRayHits", &dart::collision::RaycastResult::mRayHits);
}

} // namespace python
} // namespace dart

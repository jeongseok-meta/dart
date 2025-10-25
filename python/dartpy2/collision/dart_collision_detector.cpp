/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void dart_collision_detector(nb::module_& m)
{
  nb::class_<
      dart::collision::DARTCollisionDetector,
      dart::collision::CollisionDetector>(m, "DARTCollisionDetector")
      .def_static("create", &dart::collision::DARTCollisionDetector::create)
      .def(
          "cloneWithoutCollisionObjects",
          +[](dart::collision::DARTCollisionDetector* self)
              -> std::shared_ptr<dart::collision::CollisionDetector> {
            return self->cloneWithoutCollisionObjects();
          })
      .def(
          "getType",
          +[](const dart::collision::DARTCollisionDetector* self)
              -> const std::string& { return self->getType(); },
          nb::rv_policy::reference_internal)
      .def(
          "createCollisionGroup",
          +[](dart::collision::DARTCollisionDetector* self)
              -> std::shared_ptr<dart::collision::CollisionGroup> {
            return self->createCollisionGroup();
          })
      .def_static(
          "getStaticType",
          +[]() -> const std::string& {
            return dart::collision::DARTCollisionDetector::getStaticType();
          },
          nb::rv_policy::reference_internal);
}

} // namespace python
} // namespace dart

/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void CollisionDetector(nb::module_& m)
{
  ::nb::class_<dart::collision::CollisionDetector>(m, "CollisionDetector")
      .def(
          "cloneWithoutCollisionObjects",
          +[](dart::collision::CollisionDetector* self)
              -> std::shared_ptr<dart::collision::CollisionDetector> {
            return self->cloneWithoutCollisionObjects();
          })
      .def(
          "getType",
          +[](const dart::collision::CollisionDetector* self)
              -> const std::string& { return self->getType(); },
          nb::rv_policy::reference_internal)
      .def(
          "createCollisionGroup",
          +[](dart::collision::CollisionDetector* self)
              -> std::shared_ptr<dart::collision::CollisionGroup> {
            return self->createCollisionGroupAsSharedPtr();
          });
}

} // namespace python
} // namespace dart

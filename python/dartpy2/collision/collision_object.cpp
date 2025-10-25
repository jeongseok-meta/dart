/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void CollisionObject(nb::module_& m)
{
  ::nb::class_<dart::collision::CollisionObject>(m, "CollisionObject")
      .def(
          "getCollisionDetector",
          +[](dart::collision::CollisionObject* self)
              -> dart::collision::CollisionDetector* {
            return self->getCollisionDetector();
          },
          nb::rv_policy::reference_internal,
          "Return collision detection engine associated with this "
          "CollisionObject.")
      .def(
          "getCollisionDetector",
          +[](const dart::collision::CollisionObject* self)
              -> const dart::collision::CollisionDetector* {
            return self->getCollisionDetector();
          },
          nb::rv_policy::reference_internal,
          "Return collision detection engine associated with this "
          "CollisionObject.")
      .def(
          "getShapeFrame",
          +[](const dart::collision::CollisionObject* self)
              -> const dynamics::ShapeFrame* { return self->getShapeFrame(); },
          nb::rv_policy::reference_internal,
          "Return the associated ShapeFrame.")
      .def(
          "getShape",
          +[](const dart::collision::CollisionObject* self)
              -> dart::dynamics::ConstShapePtr { return self->getShape(); },
          "Return the associated Shape.")
      .def(
          "getTransform",
          +[](const dart::collision::CollisionObject* self)
              -> const Eigen::Isometry3d& { return self->getTransform(); },
          nb::rv_policy::reference_internal,
          "Return the transformation of this CollisionObject in world "
          "coordinates.");
}

} // namespace python
} // namespace dart

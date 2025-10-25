/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void CollisionFilter(nb::module_& m)
{
  ::nb::class_<dart::collision::CollisionFilter>(m, "CollisionFilter");

  ::nb::class_<
      dart::collision::CompositeCollisionFilter,
      dart::collision::CollisionFilter>(
      m, "CompositeCollisionFilter")
      .def(::nb::init<>())
      .def(
          "addCollisionFilter",
          +[](dart::collision::CompositeCollisionFilter* self,
              const dart::collision::CollisionFilter* filter) {
            self->addCollisionFilter(filter);
          },
          ::nb::arg("filter"),
          "Adds a collision filter to this CompositeCollisionFilter.")
      .def(
          "removeCollisionFilter",
          +[](dart::collision::CompositeCollisionFilter* self,
              const dart::collision::CollisionFilter* filter) {
            self->removeCollisionFilter(filter);
          },
          ::nb::arg("filter"),
          "Removes a collision filter from this CompositeCollisionFilter.")
      .def(
          "removeAllCollisionFilters",
          +[](dart::collision::CompositeCollisionFilter* self) {
            self->removeAllCollisionFilters();
          },
          "Removes all the collision filters from this "
          "CompositeCollisionFilter.");

  ::nb::class_<
      dart::collision::BodyNodeCollisionFilter,
      dart::collision::CollisionFilter>(
      m, "BodyNodeCollisionFilter")
      .def(::nb::init<>())
      .def(
          "addBodyNodePairToBlackList",
          +[](dart::collision::BodyNodeCollisionFilter* self,
              const dart::dynamics::BodyNode* bodyNode1,
              const dart::dynamics::BodyNode* bodyNode2) {
            self->addBodyNodePairToBlackList(bodyNode1, bodyNode2);
          },
          ::nb::arg("bodyNode1"),
          ::nb::arg("bodyNode2"),
          "Add a BodyNode pair to the blacklist.")
      .def(
          "removeBodyNodePairFromBlackList",
          +[](dart::collision::BodyNodeCollisionFilter* self,
              const dart::dynamics::BodyNode* bodyNode1,
              const dart::dynamics::BodyNode* bodyNode2) {
            self->removeBodyNodePairFromBlackList(bodyNode1, bodyNode2);
          },
          ::nb::arg("bodyNode1"),
          ::nb::arg("bodyNode2"),
          "Remove a BodyNode pair from the blacklist.")
      .def(
          "removeAllBodyNodePairsFromBlackList",
          +[](dart::collision::BodyNodeCollisionFilter* self) {
            self->removeAllBodyNodePairsFromBlackList();
          },
          "Remove all the BodyNode pairs from the blacklist.")
      .def(
          "ignoresCollision",
          +[](const dart::collision::BodyNodeCollisionFilter* self,
              const dart::collision::CollisionObject* object1,
              const dart::collision::CollisionObject* object2) -> bool {
            return self->ignoresCollision(object1, object2);
          },
          ::nb::arg("object1"),
          ::nb::arg("object2"),
          "Returns true if the given two CollisionObjects should be checked by "
          "the collision detector, false otherwise.");
}

} // namespace python
} // namespace dart

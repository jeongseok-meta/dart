/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void CollisionResult(nb::module_& m)
{
  ::nb::class_<dart::collision::CollisionResult>(m, "CollisionResult")
      .def(::nb::init<>())
      .def(
          "addContact",
          +[](dart::collision::CollisionResult* self,
              const dart::collision::Contact& contact) {
            self->addContact(contact);
          },
          ::nb::arg("contact"),
          "Add one contact.")
      .def(
          "getNumContacts",
          +[](const dart::collision::CollisionResult* self) -> std::size_t {
            return self->getNumContacts();
          },
          "Return number of contacts.")
      .def(
          "getContact",
          +[](dart::collision::CollisionResult* self, std::size_t index)
              -> dart::collision::Contact& { return self->getContact(index); },
          ::nb::arg("index"),
          ::nb::rv_policy::reference,
          "Return the index-th contact.")
      .def(
          "getContact",
          +[](const dart::collision::CollisionResult* self,
              std::size_t index) -> const dart::collision::Contact& {
            return self->getContact(index);
          },
          ::nb::arg("index"),
          ::nb::rv_policy::reference,
          "Return (const) the index-th contact.")
      .def(
          "getContacts",
          +[](const dart::collision::CollisionResult* self)
              -> const std::vector<dart::collision::Contact>& {
            return self->getContacts();
          },
          ::nb::rv_policy::reference,
          "Return contacts.")
      .def(
          "getCollidingBodyNodes",
          +[](const dart::collision::CollisionResult* self)
              -> const std::unordered_set<const dynamics::BodyNode*>& {
            return self->getCollidingBodyNodes();
          },
          ::nb::rv_policy::reference,
          "Return the set of BodyNodes that are in collision.")
      .def(
          "getCollidingShapeFrames",
          +[](const dart::collision::CollisionResult* self)
              -> const std::unordered_set<const dynamics::ShapeFrame*>& {
            return self->getCollidingShapeFrames();
          },
          ::nb::rv_policy::reference,
          "Return the set of ShapeFrames that are in collision.")
      .def(
          "inCollision",
          +[](const dart::collision::CollisionResult* self,
              const dart::dynamics::BodyNode* bn) -> bool {
            return self->inCollision(bn);
          },
          ::nb::arg("bn"),
          "Returns true if the given BodyNode is in collision.")
      .def(
          "inCollision",
          +[](const dart::collision::CollisionResult* self,
              const dart::dynamics::ShapeFrame* frame) -> bool {
            return self->inCollision(frame);
          },
          ::nb::arg("frame"),
          "Returns true if the given ShapeFrame is in collision.")
      .def(
          "isCollision",
          +[](const dart::collision::CollisionResult* self) -> bool {
            return self->isCollision();
          },
          "Return binary collision result.")
      .def(
          "clear",
          +[](dart::collision::CollisionResult* self) { self->clear(); },
          "Clear all the contacts.");
}

} // namespace python
} // namespace dart

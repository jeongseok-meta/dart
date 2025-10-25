/*
 * Copyright (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void CollisionOption(nb::module_& m)
{
  ::nb::class_<dart::collision::CollisionOption>(m, "CollisionOption")
      .def(::nb::init<>())
      .def(::nb::init<bool>(), ::nb::arg("enableContact"))
      .def(
          ::nb::init<bool, std::size_t>(),
          ::nb::arg("enableContact"),
          ::nb::arg("maxNumContacts"))
      .def(
          ::nb::init<
              bool,
              std::size_t,
              const std::shared_ptr<dart::collision::CollisionFilter>&>(),
          ::nb::arg("enableContact"),
          ::nb::arg("maxNumContacts"),
          ::nb::arg("collisionFilter"))
      .def_rw(
          "enableContact", &dart::collision::CollisionOption::enableContact)
      .def_rw(
          "maxNumContacts", &dart::collision::CollisionOption::maxNumContacts)
      .def_rw(
          "collisionFilter",
          &dart::collision::CollisionOption::collisionFilter);
}

} // namespace python
} // namespace dart

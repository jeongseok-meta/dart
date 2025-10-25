// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void Contact(nb::module_& m)
{
  ::nb::class_<dart::collision::Contact>(m, "Contact")
      .def(::nb::init<>())
      .def_static(
          "getNormalEpsilon",
          +[]() -> double {
            return dart::collision::Contact::getNormalEpsilon();
          })
      .def_static(
          "getNormalEpsilonSquared",
          +[]() -> double {
            return dart::collision::Contact::getNormalEpsilonSquared();
          })
      .def_static(
          "isZeroNormal",
          +[](const Eigen::Vector3d& normal) -> bool {
            return dart::collision::Contact::isZeroNormal(normal);
          },
          ::nb::arg("normal"))
      .def_static(
          "isNonZeroNormal",
          +[](const Eigen::Vector3d& normal) -> bool {
            return dart::collision::Contact::isNonZeroNormal(normal);
          },
          ::nb::arg("normal"))
      .def_rw("point", &dart::collision::Contact::point)
      .def_rw("normal", &dart::collision::Contact::normal)
      .def_rw("force", &dart::collision::Contact::force)
      .def_rw(
          "collisionObject1", &dart::collision::Contact::collisionObject1)
      .def_rw(
          "collisionObject2", &dart::collision::Contact::collisionObject2)
      .def_rw(
          "penetrationDepth", &dart::collision::Contact::penetrationDepth)
      .def_rw("triID1", &dart::collision::Contact::triID1)
      .def_rw("triID2", &dart::collision::Contact::triID2)
      .def_rw("userData", &dart::collision::Contact::userData);
}

} // namespace python
} // namespace dart

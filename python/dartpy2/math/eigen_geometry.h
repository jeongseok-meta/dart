/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * Eigen geometry type bindings for nanobind.
 * This file provides bindings for Eigen::Quaterniond and Eigen::Isometry3d
 * which are not natively supported by nanobind (unlike pybind11).
 *
 * Reference: python/dartpy/eigen_geometry_pybind.h (pybind11 implementation)
 */

#pragma once

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/string.h>

namespace dart {
namespace python {

namespace nb = nanobind;

/**
 * Define Eigen geometry types (Quaternion, Isometry3) with nanobind.
 * This must be called once per module to enable these types.
 *
 * Note: Unlike pybind11 which has built-in support via pybind11/eigen.h,
 * nanobind requires explicit binding of these complex Eigen types.
 */
inline void def_eigen_geometry_types(nb::module_& m)
{
  using T = double;

  // Quaternion binding
  {
    using Class = Eigen::Quaternion<T>;
    nb::class_<Class>(m, "Quaternion")
        .def(nb::init<>())
        .def(nb::init<T, T, T, T>(), nb::arg("w"), nb::arg("x"), nb::arg("y"), nb::arg("z"))
        .def("__repr__", [](const Class& self) {
          return fmt::format("Quaternion(w={}, x={}, y={}, z={})",
                             self.w(), self.x(), self.y(), self.z());
        })
        .def("w", [](const Class& self) { return self.w(); })
        .def("x", [](const Class& self) { return self.x(); })
        .def("y", [](const Class& self) { return self.y(); })
        .def("z", [](const Class& self) { return self.z(); })
        .def("coeffs", [](const Class& self) { return self.coeffs(); })
        .def("to_rotation_matrix", [](const Class& self) {
          return self.toRotationMatrix();
        })
        .def("inverse", [](const Class& self) { return self.inverse(); })
        .def("conjugate", [](const Class& self) { return self.conjugate(); })
        .def("norm", [](const Class& self) { return self.norm(); })
        .def("normalize", [](Class& self) { self.normalize(); })
        .def("normalized", [](const Class& self) { return self.normalized(); })
        .def_static("Identity", []() { return Class::Identity(); });
  }

  // Isometry3 binding
  {
    using Class = Eigen::Isometry3d;
    nb::class_<Class>(m, "Isometry3")
        .def(nb::init<>())
        .def("__repr__", [](const Class& self) {
          auto t = self.translation();
          Eigen::Quaterniond q(self.rotation());
          return fmt::format(
              "Isometry3(translation=[{}, {}, {}], rotation=Quaternion(w={}, x={}, y={}, z={}))",
              t.x(), t.y(), t.z(),
              q.w(), q.x(), q.y(), q.z());
        })
        .def("matrix", [](const Class& self) { return self.matrix(); })
        .def("translation", [](const Class& self) { return self.translation(); })
        .def("set_translation", [](Class& self, const Eigen::Vector3d& t) {
          self.translation() = t;
        })
        .def("rotation", [](const Class& self) { return self.rotation(); })
        .def("set_rotation", [](Class& self, const Eigen::Matrix3d& R) {
          self.linear() = R;
        })
        .def("set_identity", [](Class& self) { self.setIdentity(); })
        .def("inverse", [](const Class& self) { return self.inverse(); })
        .def("translate", [](Class& self, const Eigen::Vector3d& v) {
          self.translate(v);
        })
        .def("pretranslate", [](Class& self, const Eigen::Vector3d& v) {
          self.pretranslate(v);
        })
        .def_static("Identity", []() { return Class::Identity(); });
  }
}

} // namespace python
} // namespace dart

// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dartpy2 {

void Shape(nb::module_& m)
{
  // Base Shape class
  nb::class_<dart::dynamics::Shape>(m, "Shape")
      .def("get_type", &dart::dynamics::Shape::getType)
      .def("get_volume", &dart::dynamics::Shape::getVolume)
      .def("get_bounding_box", &dart::dynamics::Shape::getBoundingBox,
           nb::rv_policy::reference_internal)
      .def("get_id", &dart::dynamics::Shape::getID)
      .def("__repr__", [](const dart::dynamics::Shape& self) {
        return "Shape(type='" + self.getType() + "', volume=" + std::to_string(self.getVolume()) + ")";
      });

  // BoxShape
  nb::class_<dart::dynamics::BoxShape, dart::dynamics::Shape>(m, "BoxShape")
      .def(nb::init<const Eigen::Vector3d&>(), nb::arg("size"))
      .def("get_type", &dart::dynamics::BoxShape::getType)
      .def("set_size", &dart::dynamics::BoxShape::setSize, nb::arg("size"))
      .def("get_size", &dart::dynamics::BoxShape::getSize,
           nb::rv_policy::reference_internal)
      .def_static("get_static_type", &dart::dynamics::BoxShape::getStaticType)
      .def_static("compute_volume",
                  nb::overload_cast<const Eigen::Vector3d&>(
                      &dart::dynamics::BoxShape::computeVolume),
                  nb::arg("size"))
      .def_static("compute_inertia",
                  nb::overload_cast<const Eigen::Vector3d&, double>(
                      &dart::dynamics::BoxShape::computeInertia),
                  nb::arg("size"), nb::arg("mass"))
      .def("__repr__", [](const dart::dynamics::BoxShape& self) {
        const auto& size = self.getSize();
        return "BoxShape(size=[" + std::to_string(size.x()) + ", " +
               std::to_string(size.y()) + ", " + std::to_string(size.z()) + "])";
      });

  // SphereShape
  nb::class_<dart::dynamics::SphereShape, dart::dynamics::Shape>(m, "SphereShape")
      .def(nb::init<double>(), nb::arg("radius"))
      .def("get_type", &dart::dynamics::SphereShape::getType)
      .def("set_radius", &dart::dynamics::SphereShape::setRadius,
           nb::arg("radius"))
      .def("get_radius", &dart::dynamics::SphereShape::getRadius)
      .def_static("get_static_type", &dart::dynamics::SphereShape::getStaticType)
      .def_static("compute_volume",
                  nb::overload_cast<double>(&dart::dynamics::SphereShape::computeVolume),
                  nb::arg("radius"))
      .def_static("compute_inertia",
                  nb::overload_cast<double, double>(
                      &dart::dynamics::SphereShape::computeInertia),
                  nb::arg("radius"), nb::arg("mass"))
      .def("__repr__", [](const dart::dynamics::SphereShape& self) {
        return "SphereShape(radius=" + std::to_string(self.getRadius()) + ")";
      });

  // CylinderShape
  nb::class_<dart::dynamics::CylinderShape, dart::dynamics::Shape>(m, "CylinderShape")
      .def(nb::init<double, double>(), nb::arg("radius"), nb::arg("height"))
      .def("get_type", &dart::dynamics::CylinderShape::getType)
      .def("set_radius", &dart::dynamics::CylinderShape::setRadius,
           nb::arg("radius"))
      .def("get_radius", &dart::dynamics::CylinderShape::getRadius)
      .def("set_height", &dart::dynamics::CylinderShape::setHeight,
           nb::arg("height"))
      .def("get_height", &dart::dynamics::CylinderShape::getHeight)
      .def_static("get_static_type", &dart::dynamics::CylinderShape::getStaticType)
      .def_static("compute_volume",
                  nb::overload_cast<double, double>(
                      &dart::dynamics::CylinderShape::computeVolume),
                  nb::arg("radius"), nb::arg("height"))
      .def_static("compute_inertia",
                  nb::overload_cast<double, double, double>(
                      &dart::dynamics::CylinderShape::computeInertia),
                  nb::arg("radius"), nb::arg("height"), nb::arg("mass"))
      .def("__repr__", [](const dart::dynamics::CylinderShape& self) {
        return "CylinderShape(radius=" + std::to_string(self.getRadius()) +
               ", height=" + std::to_string(self.getHeight()) + ")";
      });
}

} // namespace dartpy2

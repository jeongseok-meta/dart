/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 */

#include <dart/constraint/BalanceConstraint.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defBalanceConstraint(nb::module_& m)
{
  // ErrorMethod enum
  nb::enum_<constraint::BalanceConstraint::ErrorMethod_t>(m, "ErrorMethod")
      .value("FROM_CENTROID", constraint::BalanceConstraint::FROM_CENTROID)
      .value("FROM_EDGE", constraint::BalanceConstraint::FROM_EDGE)
      .value("OPTIMIZE_BALANCE", constraint::BalanceConstraint::OPTIMIZE_BALANCE);

  // BalanceMethod enum
  nb::enum_<constraint::BalanceConstraint::BalanceMethod_t>(m, "BalanceMethod")
      .value("SHIFT_SUPPORT", constraint::BalanceConstraint::SHIFT_SUPPORT)
      .value("SHIFT_COM", constraint::BalanceConstraint::SHIFT_COM);

  // BalanceConstraint class
  nb::class_<constraint::BalanceConstraint>(m, "BalanceConstraint")
      .def(nb::init<
               const std::shared_ptr<dynamics::HierarchicalIK>&,
               constraint::BalanceConstraint::BalanceMethod_t,
               constraint::BalanceConstraint::ErrorMethod_t>(),
           nb::arg("ik"),
           nb::arg("balance_method") = constraint::BalanceConstraint::SHIFT_SUPPORT,
           nb::arg("error_method") = constraint::BalanceConstraint::FROM_CENTROID,
           "Create a BalanceConstraint for the given IK")
      .def("set_error_method",
           &constraint::BalanceConstraint::setErrorMethod,
           nb::arg("method"),
           "Set the error computation method")
      .def("get_error_method",
           &constraint::BalanceConstraint::getErrorMethod,
           "Get the error computation method")
      .def("set_balance_method",
           &constraint::BalanceConstraint::setBalanceMethod,
           nb::arg("method"),
           "Set the balance method")
      .def("get_balance_method",
           &constraint::BalanceConstraint::getBalanceMethod,
           "Get the balance method")
      .def("set_optimization_tolerance",
           &constraint::BalanceConstraint::setOptimizationTolerance,
           nb::arg("tolerance"),
           "Set optimization tolerance")
      .def("get_optimization_tolerance",
           &constraint::BalanceConstraint::getOptimizationTolerance,
           "Get optimization tolerance")
      .def("eval",
           &constraint::BalanceConstraint::eval,
           nb::arg("x"),
           "Evaluate the balance constraint");
}

} // namespace dart::python

/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 */

#include <dart/optimizer/Function.hpp>
#include <dart/optimizer/Problem.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

namespace nb = nanobind;

namespace dart::python {

void defProblem(nb::module_& m)
{
    // optimizer::Problem - optimization problem with objective and constraints
    nb::class_<optimizer::Problem>(m, "Problem")
        .def(nb::init<std::size_t>(),
             nb::arg("dimension") = 0,
             "Create an optimization problem with the given dimension")
        .def("set_dimension",
             &optimizer::Problem::setDimension,
             nb::arg("dimension"),
             "Set the dimension of the problem")
        .def("get_dimension",
             &optimizer::Problem::getDimension,
             "Get the dimension of the problem")
        .def("set_initial_guess",
             &optimizer::Problem::setInitialGuess,
             nb::arg("initial_guess"),
             "Set the initial guess for optimization parameters")
        .def("get_initial_guess",
             &optimizer::Problem::getInitialGuess,
             "Get the initial guess for optimization parameters")
        .def("set_lower_bounds",
             &optimizer::Problem::setLowerBounds,
             nb::arg("lower_bounds"),
             "Set the lower bounds for optimization parameters")
        .def("get_lower_bounds",
             &optimizer::Problem::getLowerBounds,
             "Get the lower bounds for optimization parameters")
        .def("set_upper_bounds",
             &optimizer::Problem::setUpperBounds,
             nb::arg("upper_bounds"),
             "Set the upper bounds for optimization parameters")
        .def("get_upper_bounds",
             &optimizer::Problem::getUpperBounds,
             "Get the upper bounds for optimization parameters")
        .def("set_objective",
             &optimizer::Problem::setObjective,
             nb::arg("objective"),
             "Set the objective function to minimize")
        .def("get_objective",
             &optimizer::Problem::getObjective,
             "Get the objective function")
          .def("add_eq_constraint",
               &optimizer::Problem::addEqConstraint,
               nb::arg("constraint"),
               "Add an equality constraint (constraint = 0)")
          .def("add_ineq_constraint",
               &optimizer::Problem::addIneqConstraint,
               nb::arg("constraint"),
               "Add an inequality constraint (constraint <= 0)")
          .def("get_eq_constraint",
               &optimizer::Problem::getEqConstraint,
               nb::arg("index"),
               "Get an equality constraint by index")
          .def("get_num_eq_constraints",
               &optimizer::Problem::getNumEqConstraints,
               "Get the number of equality constraints")
          .def("get_ineq_constraint",
               &optimizer::Problem::getIneqConstraint,
               nb::arg("index"),
               "Get an inequality constraint by index")
          .def("get_num_ineq_constraints",
               &optimizer::Problem::getNumIneqConstraints,
               "Get the number of inequality constraints")
          .def("set_optimum_value",
               &optimizer::Problem::setOptimumValue,
               nb::arg("value"),
               "Set the optimum value of the objective function")
          .def("get_optimum_value",
               &optimizer::Problem::getOptimumValue,
               "Get the optimum value of the objective function")
          .def("set_optimal_solution",
               &optimizer::Problem::setOptimalSolution,
               nb::arg("solution"),
               "Set the optimal solution")
          .def("get_optimal_solution",
               &optimizer::Problem::getOptimalSolution,
               "Get the optimal solution");
}

void defFunction(nb::module_& m)
{
  // optimizer::ModularFunction - concrete implementation for custom objectives
  // Allows setting cost and gradient functions from Python using callables
  nb::class_<optimizer::ModularFunction>(
      m, "ModularFunction")
      .def(nb::init<const std::string&>(),
           nb::arg("name") = "modular_function",
           "Create a modular optimizer function")
      .def("set_name",
           &optimizer::ModularFunction::setName,
           nb::arg("name"),
           "Set the name of this function")
      .def("get_name",
           &optimizer::ModularFunction::getName,
           "Get the name of this function")
      .def("eval",
           &optimizer::ModularFunction::eval,
           nb::arg("x"),
           "Evaluate the objective function at point x")
      .def("eval_gradient",
           [](optimizer::ModularFunction& self,
              const Eigen::VectorXd& x,
              Eigen::Ref<Eigen::VectorXd> grad) {
             Eigen::Map<Eigen::VectorXd> grad_map(grad.data(), grad.size());
             self.evalGradient(x, grad_map);
           },
           nb::arg("x"),
           nb::arg("grad"),
           "Evaluate the gradient at point x (modified in-place)")
      .def("set_cost_function",
           &optimizer::ModularFunction::setCostFunction,
           nb::arg("cost_function"),
           "Set the cost function as a Python callable: f(x) -> float")
      .def("set_gradient_function",
           &optimizer::ModularFunction::setGradientFunction,
           nb::arg("gradient_function"),
           "Set the gradient function as a Python callable: f(x, grad) -> None")
      .def("clear_cost_function",
           &optimizer::ModularFunction::clearCostFunction,
           nb::arg("print_warning") = true,
           "Clear the cost function (sets to constant zero)")
      .def("clear_gradient_function",
           &optimizer::ModularFunction::clearGradientFunction,
           "Clear the gradient function (uses default implementation)");
}

} // namespace dart::python

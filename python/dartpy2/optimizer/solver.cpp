/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/main/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include <dart/optimizer/Solver.hpp>
#include <dart/optimizer/GradientDescentSolver.hpp>

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart::python {

void defSolver(nb::module_& m)
{
  // Solver base class
  nb::class_<optimizer::Solver>(m, "Solver")
      .def("solve", &optimizer::Solver::solve,
          "Solve the optimization problem")
      .def("get_problem", &optimizer::Solver::getProblem,
          "Get the problem for this solver")
      .def("set_problem", &optimizer::Solver::setProblem,
          nb::arg("problem"),
          "Set the problem for this solver");

  // GradientDescentSolver - most common solver for IK
  nb::class_<optimizer::GradientDescentSolver, optimizer::Solver>(
      m, "GradientDescentSolver")
      .def(nb::init<const std::shared_ptr<optimizer::Problem>&>(),
          nb::arg("problem") = nullptr,
          "Construct a Gradient Descent Solver")
      .def("set_num_max_iterations",
          &optimizer::GradientDescentSolver::setNumMaxIterations,
          nb::arg("num_iterations"),
          "Set the maximum number of iterations")
      .def("get_num_max_iterations",
          &optimizer::GradientDescentSolver::getNumMaxIterations,
          "Get the maximum number of iterations")
      .def("set_step_size",
          &optimizer::GradientDescentSolver::setStepSize,
          nb::arg("step_size"),
          "Set the step size for gradient descent")
      .def("get_step_size",
          &optimizer::GradientDescentSolver::getStepSize,
          "Get the step size for gradient descent");
}

} // namespace dart::python

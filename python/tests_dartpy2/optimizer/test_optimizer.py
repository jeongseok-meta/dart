# Copyright (c) 2011-2025, The DART development contributors

import math

import dartpy2 as dart
import numpy as np
import pytest


# Problem reference: http://ab-initio.mit.edu/wiki/index.php/NLopt_Tutorial
def sample_obj_func_eval(x):
    return math.sqrt(x[1])


def sample_obj_func_eval_gradient(x, grad):
    grad[0] = 0
    grad[1] = 0.5 / (math.sqrt(x[1]) + 0.000001)


class SampleConstFunc:
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def eval(self, x):
        return (self.a * x[0] + self.b) * (self.a * x[0] + self.b) * (
            self.a * x[0] + self.b
        ) - x[1]

    def eval_gradient(self, x, grad):
        grad[0] = 3 * self.a * (self.a * x[0] + self.b) * (self.a * x[0] + self.b)
        grad[1] = -1.0


def test_gradient_descent_solver():
    prob = dart.optimizer.Problem(2)
    assert prob.get_dimension() == 2

    prob.set_lower_bounds(np.array([-1e100, 0]))
    prob.set_initial_guess(np.array([1.234, 5.678]))

    assert prob.get_initial_guess()[0] == pytest.approx(1.234)

    obj = dart.optimizer.ModularFunction()
    obj.set_cost_function(sample_obj_func_eval)
    obj.set_gradient_function(sample_obj_func_eval_gradient)
    prob.set_objective(obj)

    solver = dart.optimizer.GradientDescentSolver(prob)
    success = solver.solve()
    assert success is True

    min_f = prob.get_optimum_value()
    opt_x = prob.get_optimal_solution()

    assert min_f == pytest.approx(0, abs=1e-6)
    assert len(opt_x) == prob.get_dimension()
    assert opt_x[0] == pytest.approx(1.234, abs=0.0)
    assert opt_x[1] == pytest.approx(0, abs=solver.get_step_size())


def test_nlopt_solver():
    if not hasattr(dart.optimizer, "NloptSolver"):
        return

    prob = dart.optimizer.Problem(2)
    assert prob.get_dimension() == 2

    prob.set_lower_bounds(np.array([-1e100, 0]))
    prob.set_initial_guess(np.array([1.234, 5.678]))

    assert prob.get_initial_guess()[0] == pytest.approx(1.234)

    obj = dart.optimizer.ModularFunction()
    obj.set_cost_function(sample_obj_func_eval)
    obj.set_gradient_function(sample_obj_func_eval_gradient)
    prob.set_objective(obj)

    const1_func = SampleConstFunc(2, 0)
    const1 = dart.optimizer.ModularFunction()
    const1.set_cost_function(const1_func.eval)
    const1.set_gradient_function(const1_func.eval_gradient)
    prob.add_ineq_constraint(const1)

    const2_func = SampleConstFunc(-1, 1)
    const2 = dart.optimizer.ModularFunction()
    const2.set_cost_function(const2_func.eval)
    const2.set_gradient_function(const2_func.eval_gradient)
    prob.add_ineq_constraint(const2)

    solver = dart.optimizer.NloptSolver(prob)
    solver.set_algorithm(dart.optimizer.NloptSolver.Algorithm.LD_MMA)
    success = solver.solve()
    assert success is True

    min_f = prob.get_optimum_value()
    opt_x = prob.get_optimal_solution()

    assert min_f == pytest.approx(0.544330847, abs=1e-5)
    assert len(opt_x) == prob.get_dimension()
    assert opt_x[0] == pytest.approx(0.333334, abs=1e-5)
    assert opt_x[1] == pytest.approx(0.296296, abs=1e-5)


if __name__ == "__main__":
    pytest.main()

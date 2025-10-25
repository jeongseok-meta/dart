/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 */

#include "module.hpp"

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart::python {

void def_constraint(nb::module_& m)
{
  auto sm = m.def_submodule("constraint");
  sm.doc() = "DART Constraint module for balance and other IK constraints";

  // Register bindings
  defBalanceConstraint(sm);
}

} // namespace dart::python

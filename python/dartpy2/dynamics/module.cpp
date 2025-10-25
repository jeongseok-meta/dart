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

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart::python {

void defBodyNode(nb::module_& sm);
void defDegreeOfFreedom(nb::module_& sm);
void defEndEffector(nb::module_& sm);
void defFrame(nb::module_& sm);
void defFreeJoint(nb::module_& sm);
void defInverseKinematics(nb::module_& sm);
void defSimpleFrame(nb::module_& sm);
void defSkeleton(nb::module_& sm);
void defSupport(nb::module_& sm);

} // namespace dart::python

namespace dartpy2 {
void Shape(nb::module_& m);
}

namespace dart::python {

void def_dynamics(nb::module_& m)
{
  auto sm = m.def_submodule("dynamics");
  sm.doc() = "DART dynamics module";

  // Register bindings (order matters for dependencies)
  defFrame(sm);
  defSimpleFrame(sm);
  dartpy2::Shape(sm);  // Shape bindings
  defSupport(sm);
  defFreeJoint(sm);
  defInverseKinematics(sm);
  defEndEffector(sm);
  defBodyNode(sm);
  defDegreeOfFreedom(sm);
  defSkeleton(sm);
}

} // namespace dart::python

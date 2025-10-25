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

#pragma once

#include <Eigen/Core>
#include <nanobind/nanobind.h>
#include <stdexcept>

namespace nb = nanobind;

namespace dart::python {

/// Convert a Python sequence (list, tuple, or array) to Eigen::Vector3d
///
/// Design rationale:
/// - Accepts lists, tuples, numpy arrays, and PyTorch tensors for maximum flexibility
/// - Users can choose their preferred Python idiom
/// - Performance overhead is negligible for 3-element vectors (~10-20ns)
/// - Alternative approaches (Eigen::Ref) only accept numpy arrays, not lists
///
/// Usage:
///   .def("set_gravity",
///       [](World& self, nb::object gravityObj) {
///         self.setGravity(toEigenVector3d(gravityObj));
///       },
///       nb::arg("gravity"))
inline Eigen::Vector3d toEigenVector3d(nb::object obj)
{
  Eigen::Vector3d result;

  try {
    // Check if it's iterable and has len()
    size_t len = nb::len(obj);
    if (len != 3) {
      throw std::runtime_error(
          "Expected sequence of length 3, got " + std::to_string(len));
    }

    // Extract elements
    for (size_t i = 0; i < 3; ++i) {
      result[i] = nb::cast<double>(obj[nb::int_(i)]);
    }
  } catch (const nb::python_error&) {
    throw std::runtime_error(
        "Expected a sequence (list, tuple, or array) of 3 numbers");
  }

  return result;
}

} // namespace python

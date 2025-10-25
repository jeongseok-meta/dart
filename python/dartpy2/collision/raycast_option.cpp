/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void RaycastOption(nb::module_& m)
{
  ::nb::class_<dart::collision::RaycastOption>(m, "RaycastOption")
      .def(::nb::init<>())
      .def(::nb::init<bool>(), ::nb::arg("enableAllHits"))
      .def(
          ::nb::init<bool, bool>(),
          ::nb::arg("enableAllHits"),
          ::nb::arg("sortByClosest"))
      .def_rw(
          "mEnableAllHits", &dart::collision::RaycastOption::mEnableAllHits)
      .def_rw(
          "mSortByClosest", &dart::collision::RaycastOption::mSortByClosest);
}

} // namespace python
} // namespace dart

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

#include <dart/common/Uri.hpp>

#include <fmt/format.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void Uri(nb::module_& m)
{
  nb::class_<common::Uri>(m, "Uri")
      .def(nb::init<>())
      .def(nb::init<const std::string&>(), nb::arg("input"))
      .def("__repr__", [](const common::Uri& self) {
        return fmt::format("Uri('{}')", self.toString());
      })
      .def("clear", &common::Uri::clear)
      .def("from_string", &common::Uri::fromString, nb::arg("input"))
      .def("from_path", &common::Uri::fromPath, nb::arg("path"))
      .def("from_string_or_path", &common::Uri::fromStringOrPath, nb::arg("input"))
      .def("to_string", &common::Uri::toString)
      .def("get_path", &common::Uri::getPath)
      .def("get_filesystem_path", &common::Uri::getFilesystemPath)
      .def_static("create_from_string", &common::Uri::createFromString, nb::arg("input"))
      .def_static("create_from_path", &common::Uri::createFromPath, nb::arg("path"))
      .def_static("create_from_string_or_path", &common::Uri::createFromStringOrPath, nb::arg("input"))
      .def_static("get_uri", &common::Uri::getUri, nb::arg("input"))
      .def_rw("m_scheme", &common::Uri::mScheme)
      .def_rw("m_authority", &common::Uri::mAuthority)
      .def_rw("m_path", &common::Uri::mPath)
      .def_rw("m_query", &common::Uri::mQuery)
      .def_rw("m_fragment", &common::Uri::mFragment);
}

} // namespace python
} // namespace dart

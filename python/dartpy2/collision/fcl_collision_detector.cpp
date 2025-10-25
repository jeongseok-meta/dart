/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

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

#include <dart/dart.hpp>

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart {
namespace python {

void FCLCollisionDetector(nb::module_& m)
{
  auto fclCollisionDetector
      = nb::class_<
            dart::collision::FCLCollisionDetector,
            dart::collision::CollisionDetector>(m, "FCLCollisionDetector")
            .def_static("create", &dart::collision::FCLCollisionDetector::create)
            .def(
                "cloneWithoutCollisionObjects",
                [](dart::collision::FCLCollisionDetector* self)
                    -> std::shared_ptr<dart::collision::CollisionDetector> {
                  return self->cloneWithoutCollisionObjects();
                })
            .def(
                "getType",
                [](const dart::collision::FCLCollisionDetector* self)
                    -> const std::string& { return self->getType(); },
                nb::rv_policy::reference_internal)
            .def(
                "createCollisionGroup",
                [](dart::collision::FCLCollisionDetector* self)
                    -> std::shared_ptr<dart::collision::CollisionGroup> {
                  return self->createCollisionGroup();
                })
            .def(
                "setPrimitiveShapeType",
                [](dart::collision::FCLCollisionDetector* self,
                    dart::collision::FCLCollisionDetector::PrimitiveShape
                        type) { self->setPrimitiveShapeType(type); },
                nb::arg("type"))
            .def(
                "getPrimitiveShapeType",
                [](const dart::collision::FCLCollisionDetector* self)
                    -> dart::collision::FCLCollisionDetector::PrimitiveShape {
                  return self->getPrimitiveShapeType();
                })
            .def(
                "setContactPointComputationMethod",
                [](dart::collision::FCLCollisionDetector* self,
                    dart::collision::FCLCollisionDetector::
                        ContactPointComputationMethod method) {
                  self->setContactPointComputationMethod(method);
                },
                nb::arg("method"))
            .def(
                "getContactPointComputationMethod",
                [](const dart::collision::FCLCollisionDetector* self)
                    -> dart::collision::FCLCollisionDetector::
                        ContactPointComputationMethod {
                          return self->getContactPointComputationMethod();
                        })
            .def_static(
                "getStaticType",
                []() -> const std::string& {
                  return dart::collision::FCLCollisionDetector::getStaticType();
                },
                nb::rv_policy::reference_internal);

  nb::enum_<dart::collision::FCLCollisionDetector::PrimitiveShape>(
      fclCollisionDetector, "PrimitiveShape")
      .value(
          "PRIMITIVE",
          dart::collision::FCLCollisionDetector::PrimitiveShape::PRIMITIVE)
      .value(
          "MESH", dart::collision::FCLCollisionDetector::PrimitiveShape::MESH);

  nb::enum_<
      dart::collision::FCLCollisionDetector::ContactPointComputationMethod>(
      fclCollisionDetector, "ContactPointComputationMethod")
      .value(
          "FCL",
          dart::collision::FCLCollisionDetector::ContactPointComputationMethod::
              FCL)
      .value(
          "DART",
          dart::collision::FCLCollisionDetector::ContactPointComputationMethod::
              DART);
}

} // namespace python
} // namespace dart

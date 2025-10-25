// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#pragma once

#include <nanobind/nanobind.h>

namespace nb = nanobind;

namespace dart {
namespace python {

// Main collision module registration function
void def_collision(nb::module_& m);

// Individual component binding functions
void Contact(nb::module_& m);

void CollisionFilter(nb::module_& m);
void CollisionObject(nb::module_& m);
void CollisionOption(nb::module_& m);
void CollisionResult(nb::module_& m);

void DistanceOption(nb::module_& m);
void DistanceResult(nb::module_& m);

void RaycastOption(nb::module_& m);
void RaycastResult(nb::module_& m);

void CollisionDetector(nb::module_& m);
void FCLCollisionDetector(nb::module_& m);
void DARTCollisionDetector(nb::module_& m);

void CollisionGroup(nb::module_& m);
void FCLCollisionGroup(nb::module_& m);
void DARTCollisionGroup(nb::module_& m);

#if HAVE_BULLET
void BulletCollisionDetector(nb::module_& m);
void BulletCollisionGroup(nb::module_& m);
#endif // HAVE_BULLET

#if HAVE_ODE
void OdeCollisionDetector(nb::module_& m);
void OdeCollisionGroup(nb::module_& m);
#endif // HAVE_ODE

} // namespace python
} // namespace dart

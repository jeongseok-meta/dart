// (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.

#include "module.hpp"

#include <dart/config.hpp>

namespace dart {
namespace python {

void def_collision(nb::module_& m)
{
  auto sm = m.def_submodule("collision", "Collision detection module");

  Contact(sm);

  CollisionFilter(sm);
  CollisionObject(sm);
  CollisionOption(sm);
  CollisionResult(sm);

  DistanceOption(sm);
  DistanceResult(sm);

  RaycastOption(sm);
  RaycastResult(sm);

  CollisionDetector(sm);
  FCLCollisionDetector(sm);
  DARTCollisionDetector(sm);

    CollisionGroup(sm);
    // Note: Derived collision group classes (FCL, DART, Bullet, ODE) are not
    // registered separately because they contain non-copyable members (unique_ptr).
    // Users create collision groups via CollisionDetector.createCollisionGroup()

    // Optional: Bullet collision detector
#ifdef DART_HAVE_BULLET
    BulletCollisionDetector(sm);
#endif

    // Optional: ODE collision detector
#ifdef DART_HAVE_ODE
    OdeCollisionDetector(sm);
#endif
}

} // namespace python
} // namespace dart

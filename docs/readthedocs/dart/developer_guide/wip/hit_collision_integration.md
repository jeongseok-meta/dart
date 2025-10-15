# HIT Collision Detection Integration Plan

**Status**: 🚧 Work in Progress
**Last Updated**: 2025-10-15
**Tracking Issue**: TBD

> **⚠️ IMPORTANT**: This document should be kept in sync with the actual implementation as we progress. Update the checklist and status as each task is completed.

---

## 📋 Overview

This document tracks the integration of the HIT (Hybrid Incremental Trees) collision detection library as the default collision detector in DART, replacing the current dependencies on FCL, Bullet, and ODE.

### Goals

1. Remove hard dependencies on external collision detection libraries (FCL, Bullet, ODE)
2. Integrate FCL source code directly into DART under a new namespace `dart::collision::hit`
3. Make HIT the default collision detector
4. Maintain backward compatibility with existing collision detection APIs
5. Ensure all existing tests pass with the new detector

---

## 🎯 High-Level Milestones

- [ ] **Milestone 1**: Setup HIT source code structure
- [ ] **Milestone 2**: Build system integration
- [ ] **Milestone 3**: Create HIT adapter classes
- [ ] **Milestone 4**: Testing and validation
- [ ] **Milestone 5**: Make HIT the default detector
- [ ] **Milestone 6**: Documentation and cleanup

---

## 📝 Detailed Task List

### Milestone 1: Setup HIT Source Code Structure

**Objective**: Copy FCL source code into DART and prepare the directory structure

- [ ] **Task 1.1**: Create directory structure
  - Create `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/hit/` directory
  - Verify directory follows DART conventions (headers and sources in same location)

- [ ] **Task 1.2**: Clone FCL repository
  - Clone https://github.com/flexible-collision-library/fcl to a temporary location
  - Identify which files/directories to copy (exclude build artifacts, tests if not needed)

- [ ] **Task 1.3**: Copy FCL source files
  - Copy header files (`.h`, `.hpp`) to `dart/collision/hit/`
  - Copy source files (`.cpp`) to `dart/collision/hit/`
  - Preserve internal directory structure if needed (e.g., `broadphase/`, `narrowphase/`)

- [ ] **Task 1.4**: Namespace transformation
  - Replace all `fcl::` namespace references with `dart::collision::hit::`
  - Update include guards (e.g., `FCL_*` to `DART_COLLISION_HIT_*`)
  - Update internal includes (e.g., `#include <fcl/...>` to `#include "dart/collision/hit/..."`)

- [ ] **Task 1.5**: License and attribution
  - Ensure FCL license is properly preserved in copied files
  - Add DART copyright headers where appropriate
  - Create `LICENSE` or `ATTRIBUTION` file in the `hit/` directory

**Dependencies**: None
**Estimated Effort**: Medium
**Priority**: High

---

### Milestone 2: Build System Integration

**Objective**: Configure CMake to build HIT as part of DART

- [ ] **Task 2.1**: Create CMakeLists.txt for HIT
  - Create `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/hit/CMakeLists.txt`
  - Define source and header files
  - Configure library target `dart-collision-hit`
  - Set up include directories

- [ ] **Task 2.2**: Update parent CMakeLists.txt
  - Add `add_subdirectory(hit)` to `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/CMakeLists.txt`
  - Update header generation to include HIT headers
  - Configure installation rules

- [ ] **Task 2.3**: Handle external dependencies
  - Identify FCL's dependencies (Eigen, CCD, octomap, etc.)
  - Verify these dependencies are available in DART or add them
  - Update CMake find_package() calls as needed

- [ ] **Task 2.4**: Build verification
  - Compile the HIT library successfully
  - Verify no symbol conflicts with existing collision libraries
  - Test in both Debug and Release modes

- [ ] **Task 2.5**: Create hit.hpp.in header (optional)
  - Similar to `fcl.hpp.in`, `ode.hpp.in`, `bullet.hpp.in`
  - Include all necessary HIT headers
  - Generate during CMake configuration

**Dependencies**: Milestone 1
**Estimated Effort**: Medium
**Priority**: High

---

### Milestone 3: Create HIT Adapter Classes

**Objective**: Implement DART adapter classes for HIT, following the pattern of FCL/Bullet/ODE adapters

- [ ] **Task 3.1**: Create HitCollisionDetector
  - File: `dart/collision/hit/HitCollisionDetector.hpp`
  - File: `dart/collision/hit/HitCollisionDetector.cpp`
  - Inherit from `CollisionDetector`
  - Implement required virtual methods:
    - `cloneWithoutCollisionObjects()`
    - `getType()` / `getStaticType()`
    - `createCollisionGroup()`
    - `collide()` (both overloads)
    - `distance()` (both overloads)
    - `raycast()` (if supported)
  - Register with factory using `Registrar<HitCollisionDetector>`

- [ ] **Task 3.2**: Create HitCollisionGroup
  - File: `dart/collision/hit/HitCollisionGroup.hpp`
  - File: `dart/collision/hit/HitCollisionGroup.cpp`
  - Inherit from `CollisionGroup`
  - Manage HIT's internal collision group structures
  - Implement required virtual methods

- [ ] **Task 3.3**: Create HitCollisionObject
  - File: `dart/collision/hit/HitCollisionObject.hpp`
  - File: `dart/collision/hit/HitCollisionObject.cpp`
  - Inherit from `CollisionObject`
  - Wrap HIT collision objects
  - Handle shape frame updates

- [ ] **Task 3.4**: Create HitTypes (optional)
  - File: `dart/collision/hit/HitTypes.hpp`
  - File: `dart/collision/hit/HitTypes.cpp`
  - Define type aliases and helper types
  - Similar to `FCLTypes.hpp` / `BulletTypes.hpp`

- [ ] **Task 3.5**: Handle shape conversions
  - Implement conversion from DART shapes to HIT shapes
  - Support all DART shape types:
    - BoxShape
    - SphereShape
    - CylinderShape
    - CapsuleShape
    - ConeShape
    - PlaneShape
    - MeshShape
    - SoftMeshShape
    - etc.

- [ ] **Task 3.6**: Update CMakeLists.txt
  - Add adapter source/header files to HIT CMakeLists.txt
  - Ensure proper linking

**Dependencies**: Milestone 2
**Estimated Effort**: Large
**Priority**: High

---

### Milestone 4: Testing and Validation

**Objective**: Ensure HIT passes all existing collision tests and add HIT-specific tests

- [ ] **Task 4.1**: Enable HIT in existing tests
  - Update `/home/jeongseok/dev/dartsim/dart/coll/tests/integration/test_Collision.cpp`
  - Add HIT test cases alongside FCL/Bullet/ODE tests
  - Use parameterized tests if possible

- [ ] **Task 4.2**: Run integration tests
  - Execute `test_Collision` with HIT detector
  - Execute `test_CollisionGroups` with HIT detector
  - Execute `test_Distance` with HIT detector
  - Execute `test_Raycast` with HIT detector (if supported)
  - Fix any failures

- [ ] **Task 4.3**: Create HIT-specific unit tests
  - Create test files in `/home/jeongseok/dev/dartsim/dart/coll/tests/integration/`
  - Test HIT-specific features and edge cases
  - Test shape conversion accuracy
  - Test performance benchmarks

- [ ] **Task 4.4**: Regression testing
  - Run full DART test suite with HIT as detector
  - Check for memory leaks (valgrind, sanitizers)
  - Verify no performance regressions

- [ ] **Task 4.5**: Cross-detector validation
  - Compare collision results between HIT and FCL
  - Ensure contact points and normals are consistent
  - Document any expected differences

**Dependencies**: Milestone 3
**Estimated Effort**: Large
**Priority**: High

---

### Milestone 5: Make HIT the Default Detector

**Objective**: Set HIT as the default collision detector in DART

- [ ] **Task 5.1**: Update CollisionDetector factory
  - Locate where default detector is registered
  - Change default from FCL to HIT
  - Ensure factory registration order is correct

- [ ] **Task 5.2**: Update documentation
  - Update user-facing docs about default collision detector
  - Add migration guide for users explicitly using FCL
  - Document any API differences

- [ ] **Task 5.3**: Update examples and tutorials
  - Review all examples in `/home/jeongseok/dev/dartsim/dart/coll/examples/`
  - Update comments/documentation referencing FCL as default
  - Verify examples run correctly with HIT

- [ ] **Task 5.4**: Update CMake options
  - Add CMake option to select default detector (e.g., `DART_DEFAULT_COLLISION_DETECTOR`)
  - Allow users to override default at build time
  - Update build documentation

- [ ] **Task 5.5**: Deprecation notices
  - Add deprecation warnings for FCL/Bullet/ODE (if appropriate)
  - Update CHANGELOG.md with breaking changes
  - Communicate changes to users

**Dependencies**: Milestone 4
**Estimated Effort**: Medium
**Priority**: Medium

---

### Milestone 6: Documentation and Cleanup

**Objective**: Complete documentation and remove unnecessary dependencies

- [ ] **Task 6.1**: API documentation
  - Document all HIT adapter classes with Doxygen comments
  - Add usage examples in header files
  - Generate and review Doxygen output

- [ ] **Task 6.2**: Update build system
  - Make FCL/Bullet/ODE optional dependencies
  - Update find_package() to be optional
  - Update installation instructions

- [ ] **Task 6.3**: Update README and CHANGELOG
  - Update main README with HIT information
  - Add comprehensive CHANGELOG entry
  - List all breaking changes

- [ ] **Task 6.4**: Performance documentation
  - Document performance characteristics of HIT vs FCL
  - Add benchmark results
  - Provide guidance on when to use which detector

- [ ] **Task 6.5**: Code review and cleanup
  - Remove debug code and TODOs
  - Ensure code style consistency
  - Final review of all changes

**Dependencies**: Milestone 5
**Estimated Effort**: Small
**Priority**: Low

---

## 🔧 Technical Details

### Directory Structure

```
dart/
├── collision/
│   ├── hit/                           # New HIT integration
│   │   ├── CMakeLists.txt
│   │   ├── HitCollisionDetector.hpp
│   │   ├── HitCollisionDetector.cpp
│   │   ├── HitCollisionGroup.hpp
│   │   ├── HitCollisionGroup.cpp
│   │   ├── HitCollisionObject.hpp
│   │   ├── HitCollisionObject.cpp
│   │   ├── HitTypes.hpp
│   │   ├── HitTypes.cpp
│   │   ├── hit.hpp.in                 # Optional aggregated header
│   │   └── [FCL source files...]      # Copied FCL sources
│   ├── fcl/                           # Existing FCL adapter (kept for compatibility)
│   ├── bullet/                        # Existing Bullet adapter
│   ├── ode/                           # Existing ODE adapter
│   └── dart/                          # Native DART detector
```

### Key Classes

- **HitCollisionDetector**: Main entry point, implements `CollisionDetector` interface
- **HitCollisionGroup**: Manages groups of collision objects
- **HitCollisionObject**: Wraps individual collision geometries
- **HitTypes**: Type definitions and utilities

### Factory Registration Pattern

```cpp
// In HitCollisionDetector.cpp
static Registrar<HitCollisionDetector> mRegistrar;

// Factory instantiation
Registrar<HitCollisionDetector> HitCollisionDetector::mRegistrar(
    HitCollisionDetector::getStaticType(),
    []() -> std::shared_ptr<CollisionDetector> {
        return HitCollisionDetector::create();
    });
```

---

## 🚀 LLM Resume Prompt

**Use this prompt when resuming work on this feature:**

```
I'm working on integrating HIT (Hybrid Incremental Trees) collision detection into the DART physics engine.

The plan is documented at: /home/jeongseok/dev/dartsim/dart/coll/docs/readthedocs/dart/developer_guide/wip/hit_collision_integration.md

Current status:
- [Describe current milestone/task]
- [List completed tasks]
- [Describe any issues or blockers]

Next steps:
- [List next 2-3 tasks from the plan]

Key context:
- HIT is created by copying FCL source code into dart/collision/hit/
- All FCL namespaces should be replaced with dart::collision::hit
- Follow the pattern of existing collision adapters (fcl/, bullet/, ode/)
- The goal is to make HIT the default collision detector
- The plan document should be updated as we progress

Please continue with [specific task or milestone].
```

---

## 📊 Progress Tracking

### Completion Status

| Milestone | Progress | Status |
|-----------|----------|--------|
| M1: Setup HIT source code | 0/5 tasks | 🔴 Not Started |
| M2: Build system integration | 0/5 tasks | 🔴 Not Started |
| M3: Create HIT adapters | 0/6 tasks | 🔴 Not Started |
| M4: Testing and validation | 0/5 tasks | 🔴 Not Started |
| M5: Make HIT default | 0/5 tasks | 🔴 Not Started |
| M6: Documentation | 0/5 tasks | 🔴 Not Started |
| **Overall** | **0/31 tasks** | **🔴 0% Complete** |

### Status Legend
- 🔴 Not Started
- 🟡 In Progress
- 🟢 Completed
- 🔵 Blocked

---

## 📌 Notes and Decisions

### Decision Log

| Date | Decision | Rationale |
|------|----------|-----------|
| 2025-10-15 | Copy FCL source rather than using as submodule | Easier to modify namespace and maintain long-term |
| 2025-10-15 | Keep FCL/Bullet/ODE adapters as optional | Maintain backward compatibility |

### Open Questions

1. Should we keep FCL as a git submodule initially and convert later?
2. What version of FCL should we copy?
3. Should HIT support all FCL features (distance, raycast, etc.)?
4. Performance benchmarks: what threshold defines acceptable performance?

### Known Issues

- None yet

---

## 🔗 References

- **FCL Repository**: https://github.com/flexible-collision-library/fcl
- **DART Repository**: https://github.com/dartsim/dart
- **Existing FCL Adapter**: `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/fcl/`
- **Existing Bullet Adapter**: `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/bullet/`
- **Existing ODE Adapter**: `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/ode/`
- **CollisionDetector Base Class**: `/home/jeongseok/dev/dartsim/dart/coll/dart/collision/CollisionDetector.hpp`

---

## ✅ Checklist for Completion

Before considering this feature complete:

- [ ] All 31 tasks in milestones are completed
- [ ] All existing tests pass with HIT as default
- [ ] Performance is acceptable (no major regressions)
- [ ] Documentation is complete and reviewed
- [ ] Code review completed
- [ ] CHANGELOG updated
- [ ] This plan document reflects final state
- [ ] Users notified of changes (if breaking)

---

**Last Updated**: 2025-10-15
**Document Owner**: [Your Name]
**Reviewers**: [List reviewers]

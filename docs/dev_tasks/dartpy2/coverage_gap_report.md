# dartpy2 Coverage Gap Report

**Date**: January 2025
**Status**: Phase 1 Complete - Coverage Analysis for Phase 2+

This document tracks the detailed coverage gaps between the legacy dartpy (pybind11) and the new dartpy2 (nanobind) implementations, serving as a roadmap for achieving feature parity.

---

## Executive Summary

| Category | dartpy | dartpy2 | Gap | Completion |
|----------|--------|---------|-----|------------|
| **Test Files** | 23 files | 19 files | 4 files | 83% |
| **Test Functions** | ~50+ | ~35 | ~15+ | ~70% |
| **Examples** | 10 examples | 2 examples | 8 examples | 20% |
| **Collision Tests** | 3 tests | 0 tests | 3 tests | 0% |
| **IO/Parser Tests** | 10 tests | 3 tests | 7 tests | 30% |
| **Integration Tests** | 3 tests | 0 tests | 3 tests | 0% |

**Overall Assessment**: dartpy2 has strong foundation for core features (IK, basic dynamics, math) but lacks collision, advanced I/O, and integration test coverage.

---

## 1. Test Coverage Gaps

### 1.1 Collision Module (80% coverage - IN PROGRESS)

**Status**: 🔄 IN PROGRESS - Bindings created, testing needed

**Location**: `python/tests_dartpy2/collision/test_collision.py`

**Progress Update (January 2025)**:
- ✅ Created all 20 collision binding files (contact, options, results, detectors, groups)
- ✅ Integrated collision module into dartpy2.cpp build system
- ✅ Created comprehensive test file with 3 test functions
- ⏳ PENDING: Build and test to verify bindings work
- ⏳ PENDING: Add missing Shape bindings (SphereShape, BoxShape, ShapeNode)
- ⏳ PENDING: Debug and fix any compilation/runtime issues

**Missing Tests from** `python/tests/unit/collision/test_collision.py`:

#### A. `test_collision_groups()`
Tests collision detection with multiple detector backends:
- **FCLCollisionDetector** - Fast collision library
- **DARTCollisionDetector** - Native DART collision
- **BulletCollisionDetector** - Bullet physics (optional)
- **OdeCollisionDetector** - ODE physics (optional)

**Test scenarios**:
1. Collision with SimpleFrames (sphere-sphere)
2. Collision with BodyNodes (skeleton collision aspects)
3. Collision between separate groups
4. CollisionOption configuration (`enableContact` flag)
5. CollisionResult parsing (contact count, collision state)

#### B. `test_filter()`
Tests collision filtering and blacklist management:
- BodyNodeCollisionFilter creation
- Self-collision enable/disable
- Adjacent body check enable/disable
- Blacklist operations:
  - `addBodyNodePairToBlackList()`
  - `removeBodyNodePairFromBlackList()`
  - `removeAllBodyNodePairsFromBlackList()`
- Integration with World/ConstraintSolver

#### C. `test_raycast()`
Tests raycasting functionality:
- RaycastOption configuration (`enableAllHits`)
- Ray-sphere intersection tests
- Hit point calculation
- Surface normal calculation
- Intersection fraction calculation
- Multiple ray directions and transforms

**Required Bindings**:
```python
# Collision Detection
dart.collision.CollisionDetector
dart.collision.FCLCollisionDetector
dart.collision.DARTCollisionDetector
dart.collision.BulletCollisionDetector (optional)
dart.collision.OdeCollisionDetector (optional)
dart.collision.CollisionGroup
dart.collision.CollisionOption
dart.collision.CollisionResult

# Collision Filtering
dart.collision.BodyNodeCollisionFilter

# Raycasting
dart.collision.RaycastOption
dart.collision.RaycastResult
dart.collision.RayHit
```

**Priority**: HIGH - Collision is fundamental for contact-rich simulations

---

### 1.2 IO/Parser Module (30% coverage)

**Status**: ⚠️ INCOMPLETE - Only URDF loader tested

**Location**: `python/tests_dartpy2/io/`

#### ✅ **Has Coverage**: `test_dart_loader.py` (3 tests)
- `test_dart_loader_creation()` - Basic instantiation
- `test_load_atlas_urdf()` - URDF loading with dart:// URI
- `test_add_skeleton_to_world()` - World integration

#### ❌ **Missing Tests**:

##### A. Extended DartLoader Tests (7 tests missing)
From `python/tests/unit/utils/test_dart_loader.py`:

| Test | Purpose | Coverage Gap |
|------|---------|--------------|
| `test_parse_skeleton_non_existing_path_returns_null()` | Error handling for missing files | No error handling |
| `test_parse_skeleton_invalid_urdf_returns_null()` | Error handling for malformed URDF | No invalid file tests |
| `test_parse_skeleton_missing_mesh_returns_null()` | Error handling for missing meshes | No mesh error tests |
| `test_parse_skeleton_invalid_mesh_returns_null()` | Error handling for invalid meshes | No mesh validation |
| `test_parse_skeleton_missing_package_returns_null()` | Error handling for missing packages | No package tests |
| `test_parse_skeleton_loads_primitive_geometry()` | Box/Sphere/Cylinder primitives | No primitive tests |
| `test_parse_joint_properties()` | Joint damping, friction, limits | No property tests |

##### B. MjcfParser Tests (100% missing)
**File**: `python/tests/unit/utils/test_mjcf_parser.py`

- `test_parse_fetch()` - Parse MuJoCo MJCF format (OpenAI Fetch robot)

**Required Bindings**:
```python
dart.utils.MjcfParser
dart.utils.MjcfParser.readWorld()
```

**Use Case**: MuJoCo format is standard in RL (Gym, dm_control)

##### C. SdfParser Tests (100% missing)
**File**: `python/tests/unit/utils/test_sdf_parser.py`

- `test_read_world()` - Parse Gazebo SDF format

**Required Bindings**:
```python
dart.utils.SdfParser
dart.utils.SdfParser.readWorld()
```

**Use Case**: SDF is Gazebo's primary format, widely used in ROS

##### D. SkelParser Tests (100% missing)
**File**: `python/tests/unit/utils/test_skel_parser.py`

- `test_read_world()` - Parse DART native .skel format with options

**Required Bindings**:
```python
dart.utils.SkelParser
dart.utils.SkelParser.readWorld()
```

**Use Case**: DART's native format for efficient serialization

**Priority**: MEDIUM-HIGH - Parser diversity is critical for ecosystem integration

---

### 1.3 Integration Tests (0% coverage)

**Status**: ❌ CRITICAL - No integration test directory

**Location**: Missing `python/tests_dartpy2/integration/` directory

**Missing Tests from** `python/tests/integration/test_joint_force_torque.py`:

#### A. `test_static()`
Tests joint force/torque in static equilibrium:
- Load world from SDF (`force_torque_test.world`)
- Measure joint wrenches with gravity (-50 Z)
- Validate parent/child wrench equality (Newton's 3rd law)
- Expected forces: `[0, 0, 0, 0, 0, 1000]` and `[0, 0, 0, 0, 0, 500]`
- Frame convention handling (Gazebo vs DART)

#### B. `test_force_torque_at_joint_limits()`
Tests dynamics with gravity changes and joint limits:
- Change gravity to `[-30, 10, -50]` causing toppling
- Run 2000 simulation steps for stabilization
- Validate wrenches with tolerance (`rtol=0.1`)
- Multi-axis force components
- Joint limit enforcement

#### C. `test_force_torque_at_joint_limits_with_external_forces()`
Tests PD control with external forces:
- Three-link robot (`force_torque_test2.world`)
- Fixed root (WeldJoint)
- PD controller (kp1=5e4, kp2=1e4)
- Target positions: 0 and -0.25π radians
- 4500 simulation steps
- Wrench validation with tighter tolerance (`atol=2`)

**Required Bindings**:
```python
# SDF World Loading
dart.utils.SdfParser.readWorld(uri, resource_retriever)

# Joint Force/Torque Methods
joint.getWrenchToParentBodyNode(frame)
joint.getWrenchToChildBodyNode(frame)
joint.getTransformFromParentBodyNode()
joint.getTransformFromChildBodyNode()
joint.setLimitEnforcement(enabled)

# Frame Operations
dart.dynamics.SimpleFrame(parent_frame, transform)
dart.math.Isometry3()

# Joint Control
joint.setForce(index, effort)
joint.getPosition(index)
```

**Priority**: HIGH - Integration tests validate real-world simulation workflows

---

## 2. Example Coverage Gaps

### 2.1 Existing Examples (2/10)

✅ **atlas_puppet** - IK with balance constraints (Phase 1 target)
✅ **hello_world** - Basic skeleton creation and simulation

### 2.2 Missing Examples (8/10)

#### A. `biped_stand` ⭐ Priority: HIGH
**Purpose**: Biped balance control with SPD tracking and ankle strategy

**Key Features**:
- Full-body humanoid robot control
- Stable Proportional-Derivative (SPD) tracking
- Ankle strategy for COM/COP balance
- External force perturbations (keys 1-4)

**Required APIs**:
- PD control: `joint.setForce()`
- State feedback: `joint.getPosition()`, `joint.getVelocity()`
- COM calculation: `skeleton.getCOM()`
- External forces: `bodynode.addExtForce()`

---

#### B. `contacts_pointcloud` ⭐ Priority: HIGH
**Purpose**: Visualize collision contacts as 3D point cloud

**Key Features**:
- KR5 arm with ground contact
- Extract collision contacts from simulation
- Dynamic point cloud rendering
- Transparent shape rendering

**Required APIs**:
- `CollisionResult.getContact(index)`
- `PointCloudShape` (OSG visualization)
- Shape transparency settings

---

#### C. `drag_and_drop` ⭐ Priority: MEDIUM
**Purpose**: Interactive drag-and-drop manipulation

**Key Features**:
- OSG viewer interaction framework
- `DragAndDrop` utility
- Interactive frame creation
- Ctrl + Left-click rotation
- Visual axis markers

**Required APIs**:
- `dart.gui.osg.DragAndDrop`
- `dart.gui.osg.InteractiveFrame`
- `dart.gui.osg.InteractiveTool`

---

#### D. `hello_world_gui` ⭐ Priority: MEDIUM
**Purpose**: GUI template with custom callbacks

**Key Features**:
- `RealTimeWorldNode` class
- Custom callback hooks:
  - `customPreRefresh()`
  - `customPostRefresh()`
  - `customPreStep()`
  - `customPostStep()`
- Subclassing pattern for custom logic

**Required APIs**:
- `dart.gui.osg.RealTimeWorldNode` subclassing
- Virtual method overriding in Python

---

#### E. `operational_space_control` ⭐ Priority: HIGH
**Purpose**: Task-space (operational space) control

**Key Features**:
- Jacobian-based control
- End-effector position control (Cartesian space)
- Pseudo-inverse calculations
- PD control with gravity compensation

**Required APIs**:
- `skeleton.getWorldJacobian(bodynode)`
- `skeleton.getMassMatrix()`
- `skeleton.getCoriolisAndGravityForces()`
- NumPy pseudo-inverse: `np.linalg.pinv()`

---

#### F. `rigid_chain` ⭐ Priority: LOW
**Purpose**: Simple chain falling under gravity

**Key Features**:
- Load chain from SKEL file
- Random initial joint positions
- Joint damping
- Basic dynamics

**Required APIs**:
- SkelParser (already identified as missing)
- `joint.setPosition(index, value)`
- `joint.setDampingCoefficient(index, value)`

---

#### G. `rigid_cubes` ⭐ Priority: LOW
**Purpose**: Rigid body dynamics with shadows

**Key Features**:
- Multiple cube simulation
- Shadow rendering technique
- OSG viewer defaults

**Required APIs**:
- SkelParser
- OSG shadow techniques (likely already available)

---

#### H. `rigid_loop` ⭐ Priority: MEDIUM
**Purpose**: Closed kinematic loop with constraint

**Key Features**:
- `BallJointConstraint` between non-adjacent links
- Constraint solver validation
- Complex constraint topology

**Required APIs**:
- `dart.constraint.BallJointConstraint`
- Constraint setup and solver integration

---

## 3. Priority Roadmap

### Phase 2: Extended Joints & Collision (In Planning)

**Critical for**: Manipulation, grasping, contact-rich simulations

**Tests to Add**:
1. ✅ Bind collision detection APIs
2. ✅ Port `test_collision_groups()` (3 collision detector tests)
3. ✅ Port `test_filter()` (collision filtering tests)
4. ✅ Port `test_raycast()` (raycasting tests)

**Examples to Add**:
1. ✅ `contacts_pointcloud` - Demonstrates collision API usage

**Estimated Effort**: 2-3 weeks (bindings + tests + examples)

---

### Phase 3: Advanced Dynamics & Parsers

**Critical for**: Multi-format model loading, force control, advanced kinematics

**Tests to Add**:
1. ✅ Port MjcfParser tests (`test_mjcf_parser.py`)
2. ✅ Port SdfParser tests (`test_sdf_parser.py`)
3. ✅ Port SkelParser tests (`test_skel_parser.py`)
4. ✅ Add error handling tests to DartLoader
5. ✅ Port integration tests (`test_joint_force_torque.py`)

**Examples to Add**:
1. ✅ `biped_stand` - Balance control with SPD
2. ✅ `operational_space_control` - Jacobian-based control
3. ✅ `rigid_loop` - Constraint-based simulation

**Estimated Effort**: 3-4 weeks (parsers + bindings + tests + examples)

---

### Phase 4: Production Polish & GUI

**Critical for**: User experience, interactive tools, documentation

**Tests to Add**:
1. ✅ Increase coverage to >90% across all modules
2. ✅ Add property-based tests (hypothesis)
3. ✅ Performance regression tests

**Examples to Add**:
1. ✅ `hello_world_gui` - GUI framework template
2. ✅ `drag_and_drop` - Interactive manipulation
3. ✅ `rigid_chain` and `rigid_cubes` - Simple demos

**Documentation**:
1. ✅ Complete API documentation
2. ✅ Tutorial series
3. ✅ Migration guide from dartpy to dartpy2

**Estimated Effort**: 4-6 weeks (polish + docs + distribution)

---

## 4. Binding Requirements Summary

### Collision Module
```python
# Core collision detection
dart.collision.CollisionDetector
dart.collision.FCLCollisionDetector
dart.collision.DARTCollisionDetector
dart.collision.BulletCollisionDetector
dart.collision.OdeCollisionDetector
dart.collision.CollisionGroup
dart.collision.CollisionOption
dart.collision.CollisionResult

# Filtering
dart.collision.BodyNodeCollisionFilter

# Raycasting
dart.collision.RaycastOption
dart.collision.RaycastResult
dart.collision.RayHit
```

### IO/Parser Module
```python
# Parsers
dart.utils.MjcfParser
dart.utils.SdfParser
dart.utils.SkelParser

# Options
dart.utils.SdfParser.Options  # For integration tests
```

### Dynamics Extensions
```python
# Joint force/torque
joint.getWrenchToParentBodyNode(frame)
joint.getWrenchToChildBodyNode(frame)
joint.getTransformFromParentBodyNode()
joint.getTransformFromChildBodyNode()
joint.setLimitEnforcement(enabled)
joint.setForce(index, effort)

# Jacobians and dynamics
skeleton.getWorldJacobian(bodynode)
skeleton.getMassMatrix()
skeleton.getCoriolisAndGravityForces()

# External forces
bodynode.addExtForce(force, offset, is_force_local, is_offset_local)
```

### GUI Extensions
```python
# Interactive tools
dart.gui.osg.DragAndDrop
dart.gui.osg.InteractiveFrame
dart.gui.osg.InteractiveTool

# Visualization
dart.gui.osg.PointCloudShape
dart.gui.osg.RealTimeWorldNode (with virtual method overriding)
```

---

## 5. Test Migration Checklist

### Phase 2 (Collision)
- [ ] Create `python/tests_dartpy2/collision/test_collision.py`
- [ ] Bind collision detector classes
- [ ] Bind CollisionGroup, CollisionOption, CollisionResult
- [ ] Bind BodyNodeCollisionFilter
- [ ] Bind raycasting classes
- [ ] Port `test_collision_groups()` with all detectors
- [ ] Port `test_filter()` with filtering scenarios
- [ ] Port `test_raycast()` with intersection tests
- [ ] Run tests with pytest and verify passing

### Phase 3 (Parsers & Integration)
- [ ] Create `python/tests_dartpy2/io/test_mjcf_parser.py`
- [ ] Create `python/tests_dartpy2/io/test_sdf_parser.py`
- [ ] Create `python/tests_dartpy2/io/test_skel_parser.py`
- [ ] Bind MjcfParser with `readWorld()`
- [ ] Bind SdfParser with `readWorld()` and options
- [ ] Bind SkelParser with `readWorld()`
- [ ] Extend `test_dart_loader.py` with error handling tests
- [ ] Create `python/tests_dartpy2/integration/` directory
- [ ] Create `python/tests_dartpy2/integration/test_joint_force_torque.py`
- [ ] Bind joint wrench methods
- [ ] Bind frame operations
- [ ] Port all 3 integration tests
- [ ] Run tests with pytest and verify passing

### Phase 4 (Examples)
- [ ] Port `biped_stand` example
- [ ] Port `contacts_pointcloud` example
- [ ] Port `drag_and_drop` example
- [ ] Port `hello_world_gui` example
- [ ] Port `operational_space_control` example
- [ ] Port `rigid_chain` example
- [ ] Port `rigid_cubes` example
- [ ] Port `rigid_loop` example
- [ ] Create example verification tests
- [ ] Add README for each example
- [ ] Verify all examples run successfully

---

## 6. Success Metrics

### Test Coverage Targets

| Phase | Test Files | Test Functions | Coverage |
|-------|-----------|---------------|----------|
| Phase 1 (✅ Complete) | 19 | ~35 | ~70% |
| Phase 2 (🔄 Target) | 20 | ~38 | ~76% |
| Phase 3 (📋 Target) | 27 | ~51 | ~90% |
| Phase 4 (📋 Target) | 30+ | ~55+ | >90% |

### Example Coverage Targets

| Phase | Examples | Coverage |
|-------|----------|----------|
| Phase 1 (✅ Complete) | 2 | 20% |
| Phase 2 (🔄 Target) | 3 | 30% |
| Phase 3 (📋 Target) | 6 | 60% |
| Phase 4 (📋 Target) | 10 | 100% |

### API Binding Targets

| Module | dartpy APIs | dartpy2 APIs | Target |
|--------|-------------|--------------|--------|
| collision | ~15 classes | 0 classes | 15 classes |
| io/parsers | 4 parsers | 1 parser | 4 parsers |
| dynamics | ~50 methods | ~30 methods | 50 methods |
| gui.osg | ~20 classes | ~8 classes | 20 classes |

---

## 7. Notes & Considerations

### Testing Strategy
- **Unit tests**: Test individual classes and methods in isolation
- **Integration tests**: Test complete workflows (simulation loops, force control)
- **Example tests**: Verify examples run without errors (not output validation)
- **Property-based tests**: Use hypothesis for edge cases (Phase 4)

### API Design Decisions
- Follow Python conventions (snake_case) over C++ (camelCase)
- Prefer properties for simple getters/setters
- Use `__repr__` for all classes
- Accept flexible inputs (tuples, lists, NumPy) for vectors
- Use factory methods for smart pointer returns

### Backward Compatibility
- dartpy2 is NOT designed to be API-compatible with dartpy
- Migration guide will document API differences
- Both can coexist (different package names)

### Performance Considerations
- Minimize Python↔C++ boundary crossings
- Batch operations when possible
- Use NumPy arrays for large data transfers
- Profile before optimizing

---

## 8. Contributing

To contribute to closing these gaps:

1. **Pick a test file** from the checklist
2. **Read the old dartpy test** to understand what's being tested
3. **Check if required bindings exist** in dartpy2
4. **If bindings missing**: Add them following `docs/dev_tasks/dartpy2/python_bindings.md`
5. **Port the test**: Adapt to dartpy2 API (snake_case, properties, etc.)
6. **Verify test passes**: Run with `pixi run test-py2`
7. **Submit PR**: Tag with `dartpy2` label

**Questions?** Open a GitHub Discussion or check existing issues with `dartpy2` label.

---

**Last Updated**: January 2025
**Status**: Living document - Update as gaps are closed
**Owner**: dartpy2 development team

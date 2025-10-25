# dartpy2 - Complete Atlas Puppet Implementation Plan

**Goal**: Achieve **COMPLETE** feature parity with C++ atlas_puppet example
**Status**: GUI Basic ✅ | IK Core ✅ | Advanced Features ⏳
**Date**: 2025-10-12
**Principle**: NO LIMITED VERSIONS - Full functionality or nothing

---

## 🎯 FINAL GOAL

Make `/home/jeongseok/dev/dartsim/dart_ci/python/examples_dartpy2/atlas_puppet/main.py` have **THE SAME OR BETTER** features as `/home/jeongseok/dev/dartsim/dart_ci/examples/atlas_puppet/main.cpp`

### C++ Example Features (ALL REQUIRED):
1. ✅ Load Atlas robot from URDF
2. ✅ Create end effectors for hands and feet
3. ✅ Set up IK with targets
4. ✅ Support geometry for balance
5. ⏳ **Custom objective function (RelaxedPosture)**
6. ⏳ **Balance constraint for COM optimization**
7. ⏳ **Interactive manipulation (drag-and-drop)**
8. ⏳ **Keyboard controls (WASD/QE for movement)**
9. ⏳ **Real-time IK solving in custom WorldNode**
10. ⏳ **Support polygon visualization**
11. ⏳ **Event handling system**
12. ⏳ **Camera controls**

---

## ✅ COMPLETED FEATURES

### Phase 1: Core IK Infrastructure (Session 1-2)
- ✅ InverseKinematics class with solve_and_apply()
- ✅ IK configuration (hierarchy, whole body)
- ✅ Target setting (SimpleFrame)
- ✅ ErrorMethod (linear/angular bounds)
- ✅ GradientMethod (component weights)
- ✅ EndEffector transforms
- ✅ 66/66 tests passing

### Phase 2: Support & Basic GUI (Session 3)
- ✅ Support class for foot balance
- ✅ Support geometry setting
- ✅ EndEffector.get_support()
- ✅ GUI module structure
- ✅ InteractiveFrame bindings
- ✅ Viewer basic bindings
- ✅ WorldNode bindings
- ⚠️ **GUI NOT TESTED YET** - Need to verify window opens!

---

## 🚧 MISSING FEATURES (Blocking Full Parity)

### Category 1: Optimizer & Constraints (CRITICAL)
**Without these, we cannot implement RelaxedPosture and BalanceConstraint**

#### 1.1 dart::optimizer::Function
```cpp
// C++ (examples/atlas_puppet/main.cpp lines 48-130)
class RelaxedPosture : public dart::optimizer::Function
{
  double eval(const Eigen::VectorXd& _x) override;
  void evalGradient(const Eigen::VectorXd& _x, Eigen::Map<Eigen::VectorXd> _grad) override;
};
```

**Required Bindings**:
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/optimizer/module.cpp`
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/optimizer/function.cpp`
- Bind `dart::optimizer::Function` as abstract base class
- Support Python subclassing via trampoline class
- Bind `eval()` and `evalGradient()` as pure virtual

**Python API Target**:
```python
class RelaxedPosture(dart.optimizer.Function):
    def eval(self, x: np.ndarray) -> float:
        # Custom objective
        return 0.5 * np.dot(result, result)

    def eval_gradient(self, x: np.ndarray, grad: np.ndarray) -> None:
        # Fill gradient array
        grad[:] = result
```

#### 1.2 dart::constraint::BalanceConstraint
```cpp
// C++ (lines 804-805, 822-823)
balance = std::make_shared<dart::constraint::BalanceConstraint>(atlas->getIK());
balance->setErrorMethod(dart::constraint::BalanceConstraint::FROM_CENTROID);
balance->setBalanceMethod(dart::constraint::BalanceConstraint::SHIFT_SUPPORT);
```

**Required Bindings**:
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/constraint/module.cpp`
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/constraint/balance_constraint.cpp`
- Bind `BalanceConstraint` class
- Bind `ErrorMethod` enum (FROM_CENTROID, FROM_EDGE, OPTIMIZE_BALANCE)
- Bind `BalanceMethod` enum (SHIFT_COM, SHIFT_SUPPORT)
- Bind `setErrorMethod()`, `setBalanceMethod()`

**Python API Target**:
```python
balance = dart.constraint.BalanceConstraint(atlas.get_ik())
balance.set_error_method(dart.constraint.BalanceConstraint.ErrorMethod.FROM_CENTROID)
balance.set_balance_method(dart.constraint.BalanceConstraint.BalanceMethod.SHIFT_SUPPORT)
ik.get_problem().add_eq_constraint(balance)
```

---

### Category 2: GUI Advanced Features (HIGH PRIORITY)

#### 2.1 Custom WorldNode with Update Callbacks
```cpp
// C++ (lines 132-253)
class TeleoperationWorld : public dart::gui::osg::WorldNode
{
  void customPreRefresh() override; // Called every frame
};
```

**Required Bindings**:
- Enhance `world_node.cpp` to support Python subclassing
- Add trampoline class for `customPreRefresh()` virtual method
- Enable custom WorldNode classes in Python

**Python API Target**:
```python
class TeleoperationWorld(dart.gui.osg.WorldNode):
    def custom_pre_refresh(self):
        # Update robot state before rendering
        self.atlas.get_ik(True).solve_and_apply(True)
```

#### 2.2 Event Handler System
```cpp
// C++ (lines 255-501)
class InputHandler : public osgGA::GUIEventHandler
{
  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override;
};
viewer.addEventHandler(new InputHandler(&viewer, node, atlas, world));
```

**Required Bindings**:
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/gui/osg/event_handler.cpp`
- Bind `GUIEventHandler` base class (or DART wrapper)
- Support Python subclassing
- Bind event types (KEYDOWN, KEYUP, etc.)
- Add `Viewer.add_event_handler()` method

**Python API Target**:
```python
class InputHandler(dart.gui.osg.EventHandler):
    def handle(self, event):
        if event.event_type == EventType.KEYDOWN:
            if event.key == 'w':
                self.move_forward()
        return True

viewer.add_event_handler(InputHandler(viewer, world_node, atlas, world))
```

#### 2.3 Drag-and-Drop for BodyNodes
```cpp
// C++ (lines 841-857)
for (std::size_t i = 0; i < atlas->getNumBodyNodes(); ++i)
  viewer.enableDragAndDrop(atlas->getBodyNode(i), false, false);
```

**Required Bindings**:
- Extend `Viewer.enable_drag_and_drop()` to accept `BodyNode*`
- Currently only supports `InteractiveFrame*`

**Python API Target**:
```python
for i in range(atlas.get_num_body_nodes()):
    viewer.enable_drag_and_drop(atlas.get_body_node(i))
```

#### 2.4 Support Polygon Visualization
```cpp
// C++ (lines 891-892)
viewer.addAttachment(
    new dart::gui::osg::SupportPolygonVisual(atlas, display_elevation));
```

**Required Bindings**:
- Create `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/gui/osg/support_polygon_visual.cpp`
- Bind `SupportPolygonVisual` class
- Add `Viewer.add_attachment()` method

**Python API Target**:
```python
visual = dart.gui.osg.SupportPolygonVisual(atlas, elevation=0.05)
viewer.add_attachment(visual)
```

#### 2.5 Camera Controls
```cpp
// C++ (lines 927-933)
viewer.getCameraManipulator()->setHomePosition(...);
viewer.setCameraManipulator(viewer.getCameraManipulator());
```

**Required Bindings**:
- Add `Viewer.get_camera_manipulator()` method
- Add `Viewer.set_camera_manipulator()` method
- Bind basic camera manipulator methods

**Python API Target**:
```python
cam = viewer.get_camera_manipulator()
cam.set_home_position(eye=[5.34, 3.0, 2.41], center=[0, 0, 1], up=[-0.2, -0.08, 0.98])
viewer.set_camera_manipulator(cam)
```

#### 2.6 Simulation Control
```cpp
// C++ (line 882)
viewer.allowSimulation(false);  // Kinematic mode (IK only)
```

**Required Bindings**:
- Add `Viewer.allow_simulation()` method

**Python API Target**:
```python
viewer.allow_simulation(False)  # Disable physics
```

---

### Category 3: Joint/DOF Utilities (MEDIUM PRIORITY)

#### 3.1 FreeJoint Position Conversion
```cpp
// C++ (line 226)
mAtlas->getJoint(0)->setPositions(FreeJoint::convertToPositions(new_tf));
```

**Required Bindings**:
- Add `FreeJoint.convertToPositions()` static method
- This converts Isometry3d to 6-DOF vector

**Python API Target**:
```python
from dartpy2.dynamics import FreeJoint
positions = FreeJoint.convert_to_positions(transform)
atlas.get_joint(0).set_positions(positions)
```

---

## 📋 IMPLEMENTATION PHASES

### 🔴 Phase 3: Optimizer & Constraints (CRITICAL - START HERE)
**Estimated**: 8-10 hours
**Blocks**: Full atlas_puppet functionality

**Tasks**:
1. ✅ Test GUI window creation (verify current implementation works)
2. Create `python/dartpy2/optimizer/` module
   - `module.cpp/hpp`
   - `function.cpp` - Bind optimizer::Function with Python subclassing
   - `gradient_descent_solver.cpp` - Bind GradientDescentSolver
3. Create `python/dartpy2/constraint/` module
   - `module.cpp/hpp`
   - `balance_constraint.cpp` - Bind BalanceConstraint
4. Update `CMakeLists.txt` to include new modules
5. Update `dartpy2.cpp` to register modules
6. Create tests: `test_optimizer.py`, `test_balance_constraint.py`
7. Implement `RelaxedPosture` in Python atlas_puppet

**Success Criteria**:
- Can create custom objective functions in Python
- Can add BalanceConstraint to IK problem
- Tests pass

---

### 🟡 Phase 4: GUI Advanced Features (HIGH)
**Estimated**: 10-12 hours
**Depends on**: Phase 3

**Tasks**:
1. Extend `world_node.cpp` for Python subclassing
   - Add trampoline for `customPreRefresh()`
   - Test custom WorldNode in Python
2. Create `event_handler.cpp`
   - Bind event handler base class
   - Support Python callbacks
   - Test keyboard events
3. Extend `viewer.cpp`
   - Add `add_event_handler()`
   - Add `allow_simulation()`
   - Add `enable_drag_and_drop()` for BodyNode
   - Add camera manipulator methods
   - Add `add_attachment()`
4. Create `support_polygon_visual.cpp`
   - Bind SupportPolygonVisual class
5. Create comprehensive tests
6. Update atlas_puppet to use all GUI features

**Success Criteria**:
- Can create custom WorldNode in Python
- Keyboard controls work
- Can drag body nodes
- Support polygon visible
- Camera controls work

---

### 🟢 Phase 5: Joint Utilities & Polish (MEDIUM)
**Estimated**: 4-6 hours
**Depends on**: Phase 4

**Tasks**:
1. Add `FreeJoint` bindings
   - Static method `convertToPositions()`
2. Polish existing bindings
3. Add comprehensive docstrings
4. Performance testing
5. Memory leak testing

**Success Criteria**:
- FreeJoint conversion works
- All features documented
- No memory leaks
- Performance acceptable

---

### 🎉 Phase 6: Final Integration & Documentation (FINAL)
**Estimated**: 4-6 hours
**Depends on**: Phase 5

**Tasks**:
1. Complete Python atlas_puppet example
   - Match ALL C++ features
   - Add detailed comments
   - Create README with controls
2. Update all documentation
   - README.md
   - migration_guide.rst
   - examples.rst
3. Create tutorial video/GIF
4. Performance benchmarks
5. Comprehensive testing
6. CI/CD verification

**Success Criteria**:
- ✅ Python atlas_puppet = C++ atlas_puppet (feature parity)
- ✅ All tests passing (target: 85+ tests)
- ✅ Documentation complete
- ✅ Examples work out of the box
- ✅ CI/CD green

---

## 📊 CURRENT SESSION PLAN

### Session 4 Tasks (TODAY):

#### Task 1: Test GUI Window Creation ⏳
**Estimated**: 30 minutes
**Priority**: CRITICAL - Must verify GUI works before adding more features

**Steps**:
1. Create simple test script to open GUI window
2. Verify window renders
3. Test basic interaction
4. Document any issues

#### Task 2: Create Optimizer Module ⏳
**Estimated**: 4 hours
**Priority**: HIGH - Blocks RelaxedPosture

**Steps**:
1. Create module directory structure
2. Bind optimizer::Function base class
3. Enable Python subclassing with trampoline
4. Bind GradientDescentSolver
5. Write tests
6. Integrate with IK

#### Task 3: Create Constraint Module ⏳
**Estimated**: 3 hours
**Priority**: HIGH - Blocks BalanceConstraint

**Steps**:
1. Create module directory structure
2. Bind BalanceConstraint class
3. Bind enums (ErrorMethod, BalanceMethod)
4. Bind Problem.addEqConstraint()
5. Write tests

#### Task 4: Update Atlas Puppet Example ⏳
**Estimated**: 2 hours
**Priority**: MEDIUM - Shows progress

**Steps**:
1. Implement RelaxedPosture in Python
2. Add BalanceConstraint to IK
3. Test complete IK pipeline
4. Document improvements

---

## 🗑️ FILES TO DELETE/CONSOLIDATE

After this consolidation:
- ❌ DELETE: `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/ROADMAP_IK_COMPLETE.md`
- ❌ DELETE: `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/GUI_IMPLEMENTATION_STATUS.md`
- ✅ KEEP: `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/IMPLEMENTATION_PLAN.md` (THIS FILE)

---

## 📚 REFERENCE MATERIALS

### C++ Headers to Study:
- `/home/jeongseok/dev/dartsim/dart_ci/dart/optimizer/Function.hpp`
- `/home/jeongseok/dev/dartsim/dart_ci/dart/optimizer/GradientDescentSolver.hpp`
- `/home/jeongseok/dev/dartsim/dart_ci/dart/constraint/BalanceConstraint.hpp`
- `/home/jeongseok/dev/dartsim/dart_ci/dart/gui/osg/Viewer.hpp`
- `/home/jeongseok/dev/dartsim/dart_ci/dart/gui/osg/WorldNode.hpp`

### Example Code:
- C++: `/home/jeongseok/dev/dartsim/dart_ci/examples/atlas_puppet/main.cpp`
- Python (current): `/home/jeongseok/dev/dartsim/dart_ci/python/examples_dartpy2/atlas_puppet/main.py`

### Existing Bindings (for reference):
- pybind11 GUI: `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy/gui/`
- nanobind patterns: `/home/jeongseok/dev/dartsim/dart_ci/python/dartpy2/dynamics/`

---

## ✅ DEFINITION OF DONE

### For "Complete Feature Parity":
1. ✅ All IK features from C++ example work in Python
2. ⏳ Custom objective functions (RelaxedPosture) working
3. ⏳ Balance constraints working
4. ⏳ Interactive GUI with drag-and-drop
5. ⏳ Keyboard controls functional
6. ⏳ Support polygon visualization
7. ⏳ Real-time IK solving
8. ⏳ Camera controls
9. ⏳ 85+ tests passing
10. ⏳ Documentation complete

### Success Metrics:
- **Functionality**: 100% feature parity with C++ atlas_puppet
- **Performance**: < 10% overhead vs C++
- **Usability**: Clear documentation, works out of the box
- **Quality**: All tests pass, no memory leaks
- **Maintainability**: Clean code, follows conventions

---

## 🚀 NEXT IMMEDIATE STEPS

1. **Test GUI** - Create simple test to verify window opens
2. **Implement optimizer module** - Enable custom objective functions
3. **Implement constraint module** - Enable balance constraints
4. **Update atlas_puppet** - Add new features as they're implemented
5. **Document progress** - Keep this plan updated

**Let's make dartpy2 the BEST Python robotics library with FULL C++ feature parity!** 🤖✨

---

**Last Updated**: 2025-10-12
**Status**: Phase 3 Starting - Optimizer & Constraints

# Atlas Puppet - Interactive IK Example

A complete Python port of the C++ `atlas_puppet` example, demonstrating interactive whole-body inverse kinematics for the Atlas humanoid robot using dartpy2.

## Overview

This example showcases:
- 🤖 **Humanoid Robot Manipulation** - Full Atlas robot with 30 DOFs
- 🎯 **Interactive IK Targets** - Drag-and-drop control for hands and feet
- 🦾 **Whole-Body IK** - Coordinated full-body motion solving
- 👣 **Support Geometry** - Balance and contact point computation
- 🎮 **3D GUI** - Interactive visualization with OpenSceneGraph

## Running the Example

```bash
# Using pixi (recommended)
pixi run py2-ex-atlas-puppet

# Or directly with Python
cd /path/to/dart_ci
PYTHONPATH=build/default/cpp/Release/python/dartpy2 python python/examples_dartpy2/atlas_puppet/main.py
```

## What It Does

### 1. Robot Setup
- Loads Atlas humanoid robot from URDF
- Sets initial crouching configuration
- Configures 30 degrees of freedom

### 2. End Effector Creation
Creates 4 interactive end effectors:
- **Left Hand**: Unconstrained whole-body IK
- **Right Hand**: Unconstrained whole-body IK
- **Left Foot**: Ground-constrained with support geometry
- **Right Foot**: Ground-constrained with support geometry

### 3. Interactive Manipulation
- Each end effector has a draggable 3D frame target
- Moving targets triggers real-time IK solving
- Robot smoothly follows target positions

### 4. IK Configuration
- **Hierarchy Levels**: Feet use secondary IK (level 1) to support hand motion
- **Component Weights**: Lower weights on floating base (DOFs 0-5)
- **Constraints**: Feet constrained to stay flat on ground

## Interactive Controls

| Control | Action |
|---------|--------|
| **Mouse + Drag** | Move IK target frames (hands/feet) |
| **Mouse Wheel** | Zoom in/out |
| **Right Mouse + Drag** | Rotate camera view |
| **Middle Mouse + Drag** | Pan camera |

## Code Structure

```python
# Main workflow
def main():
    # 1. Create world and load Atlas
    world = dart.simulation.World()
    atlas = create_atlas()

    # 2. Setup initial pose
    setup_start_configuration(atlas)

    # 3. Create end effectors with IK
    ik_targets = setup_end_effectors(atlas)

    # 4. Create GUI and add world
    viewer = dart.gui.osg.Viewer()
    world_node = dart.gui.osg.WorldNode(world)
    viewer.add_world_node(world_node)

    # 5. Enable drag-and-drop for targets
    for target in ik_targets.values():
        viewer.enable_drag_and_drop(target)

    # 6. Run interactive GUI
    viewer.run()
```

## Key Features Demonstrated

### 1. URDF Loading
```python
loader = dart.io.DartLoader()
atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")
```

### 2. Joint Configuration
```python
atlas.get_dof("r_leg_kny").set_position(90.0 * np.pi / 180.0)
```

### 3. End Effector Creation
```python
l_hand = l_hand_node.create_end_effector("l_hand")
l_hand.setDefaultRelativeTransform(tf_hand, l_hand_node)
```

### 4. IK Setup
```python
l_hand_ik = l_hand.get_ik(create_if_null=True)
l_hand_ik.set_target(lh_target)
l_hand_ik.use_whole_body()
```

### 5. Support Geometry
```python
l_foot_support = l_foot.get_support(create_if_null=True)
l_foot_support.setGeometry(support_geometry)
l_foot_support.setActive()
```

### 6. Interactive Frames
```python
lh_target = dart.gui.osg.InteractiveFrame(dart.dynamics.Frame.World(), "lh_target")
viewer.enable_drag_and_drop(lh_target)
```

## Technical Details

### IK Hierarchy
- **Level 0** (Default): Hand IK - Primary objectives
- **Level 1**: Foot IK - Secondary objectives (don't interfere with hands)

### Component Weights
```python
weights = np.ones(atlas.get_num_dofs())
weights[0:6] = 0.1  # Lower weight for floating base DOFs
l_hand_ik.get_gradient_method().set_component_weights(weights)
```

### Error Bounds
- **Hands**: Large bounds (±1000) - unconstrained movement
- **Feet**:
  - Linear: Z-axis locked (±1e-8) - stay on ground
  - Angular: X/Y locked (±1e-8) - stay flat

## Comparison with C++ Version

| Feature | C++ | Python | Status |
|---------|-----|--------|--------|
| Robot Loading | ✓ | ✓ | ✅ Complete |
| Initial Config | ✓ | ✓ | ✅ Complete |
| Hand EndEffectors | ✓ | ✓ | ✅ Complete |
| Foot EndEffectors | ✓ | ✓ | ✅ Complete |
| IK Setup | ✓ | ✓ | ✅ Complete |
| Support Geometry | ✓ | ✓ | ✅ Complete |
| Interactive GUI | ✓ | ✓ | ✅ Complete |
| Drag-and-Drop | ✓ | ✓ | ✅ Complete |
| **Lines of Code** | 938 | 285 | **70% less!** |

## Learning Resources

### Related Examples
- `hello_world/` - Basic dartpy2 usage
- `atlas_puppet_simple/` - Simplified version without GUI

### Documentation
- [dartpy2 User Guide](../../../docs/readthedocs/dartpy/user_guide/)
- [dartpy2 API Reference](../../../docs/readthedocs/dartpy/api/)
- [Python Bindings Developer Guide](../../../docs/readthedocs/dartpy/developer_guide/python_bindings.rst)

### C++ Reference
- Original C++ example: `examples/atlas_puppet/main.cpp`

## Requirements

- dartpy2 with GUI support (OpenSceneGraph)
- NumPy
- Atlas URDF files (included with DART)

## Troubleshooting

### GUI doesn't start
Make sure OpenSceneGraph is installed:
```bash
pixi install  # OSG included in dependencies
```

### Robot doesn't move when dragging
- Ensure you're dragging the colored frames (IK targets), not the robot itself
- Check terminal for any error messages

### Import errors
Make sure dartpy2 is built:
```bash
pixi run build-py2
pixi run test-py2  # Verify 66/66 tests pass
```

## License

BSD-style License - See DART's main LICENSE file

## Authors

- Python Port: dartpy2 team
- Original C++: DART development contributors

---

**Happy robot manipulation!** 🤖✨

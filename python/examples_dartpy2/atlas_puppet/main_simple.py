"""
Atlas Puppet (Simplified) - IK without Interactive Frames

This demonstrates:
- Loading the Atlas humanoid robot
- Setting up hand and foot end effectors with IK
- Running the visualization (non-interactive)
- Whole-body IK solver with RelaxedPosture + BalanceConstraint

Note: This version doesn't use InteractiveFrame due to symbol linkage issues.
For full interactive IK, use the C++ atlas_puppet example.
"""

import dartpy2 as dart
import numpy as np


def create_atlas():
    """Load the Atlas robot from URDF."""
    loader = dart.io.DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")
    return atlas


def setup_start_configuration(atlas):
    """Set initial crouching configuration."""
    # Right leg - crouching
    atlas.get_dof("r_leg_hpy").set_position(-45.0 * np.pi / 180.0)
    atlas.get_dof("r_leg_kny").set_position(90.0 * np.pi / 180.0)
    atlas.get_dof("r_leg_aky").set_position(-45.0 * np.pi / 180.0)

    # Left leg - crouching
    atlas.get_dof("l_leg_hpy").set_position(-45.0 * np.pi / 180.0)
    atlas.get_dof("l_leg_kny").set_position(90.0 * np.pi / 180.0)
    atlas.get_dof("l_leg_aky").set_position(-45.0 * np.pi / 180.0)

    # Right arm
    atlas.get_dof("r_arm_shx").set_position(65.0 * np.pi / 180.0)
    atlas.get_dof("r_arm_ely").set_position(90.0 * np.pi / 180.0)
    atlas.get_dof("r_arm_elx").set_position(-90.0 * np.pi / 180.0)
    atlas.get_dof("r_arm_wry").set_position(65.0 * np.pi / 180.0)

    # Left arm
    atlas.get_dof("l_arm_shx").set_position(-65.0 * np.pi / 180.0)
    atlas.get_dof("l_arm_ely").set_position(90.0 * np.pi / 180.0)
    atlas.get_dof("l_arm_elx").set_position(90.0 * np.pi / 180.0)
    atlas.get_dof("l_arm_wry").set_position(65.0 * np.pi / 180.0)

    # Set knee joint limits to prevent over-extension
    atlas.get_dof("r_leg_kny").set_position_limits(10.0 * np.pi / 180.0, np.pi)
    atlas.get_dof("l_leg_kny").set_position_limits(10.0 * np.pi / 180.0, np.pi)


def setup_whole_body_solver(atlas):
    """Setup whole-body IK solver with RelaxedPosture objective and BalanceConstraint."""
    ik = atlas.get_ik(create_if_null=True)
    num_dofs = atlas.get_num_dofs()

    # Set up weights (lower for floating base and torso)
    default_weight = 0.01
    weights = np.full(num_dofs, default_weight)
    weights[2] = 0.0  # DOF 2
    weights[3] = 0.0  # DOF 3
    weights[4] = 0.0  # DOF 4
    weights[6] *= 0.2  # Torso
    weights[7] *= 0.2
    weights[8] *= 0.2

    # Lower posture bounds
    lower_posture = np.full(num_dofs, -np.inf)
    lower_posture[0] = -0.35
    lower_posture[1] = -0.35
    lower_posture[5] = 0.600  # Root height
    lower_posture[6:9] = -0.1  # Torso bounds

    # Upper posture bounds
    upper_posture = np.full(num_dofs, np.inf)
    upper_posture[0] = 0.35
    upper_posture[1] = 0.35
    upper_posture[5] = 0.885
    upper_posture[6:9] = 0.1

    # Create RelaxedPosture objective using ModularFunction
    ideal_posture = atlas.get_positions()

    def compute_result_vector(x):
        """Helper to compute result vector for gradient."""
        result = np.zeros(len(x))
        for i in range(min(len(x), len(ideal_posture))):
            if x[i] < lower_posture[i]:
                result[i] = weights[i] * (x[i] - lower_posture[i])
            elif x[i] > upper_posture[i]:
                result[i] = weights[i] * (x[i] - upper_posture[i])
        return result

    def relaxed_posture_cost(x):
        """Cost function: 0.5 * ||result||^2"""
        result = compute_result_vector(x)
        return 0.5 * np.dot(result, result)

    def relaxed_posture_gradient(x, grad):
        """Gradient function (in-place)."""
        result = compute_result_vector(x)
        grad[:] = 0.0
        smaller = min(len(result), len(grad))
        grad[:smaller] = result[:smaller]

    # Create and set objective
    objective = dart.optimizer.ModularFunction("RelaxedPosture")
    objective.set_cost_function(relaxed_posture_cost)
    objective.set_gradient_function(relaxed_posture_gradient)
    ik.set_objective(objective)

    # Create BalanceConstraint for COM stability
    balance = dart.constraint.BalanceConstraint(
        ik,
        balance_method=dart.constraint.BalanceMethod.SHIFT_SUPPORT,
        error_method=dart.constraint.ErrorMethod.FROM_CENTROID
    )

    # Add balance constraint to IK problem
    problem = ik.get_problem()
    problem.add_eq_constraint(balance)

    print("✓ Whole-body solver configured with RelaxedPosture + BalanceConstraint")


def setup_end_effectors(atlas):
    """Create end effectors for hands and feet with IK (using SimpleFrame targets)."""
    # IK bounds for hands (infinite - unconstrained initially)
    linear_bounds = np.array([1000.0, 1000.0, 1000.0])
    angular_bounds = np.array([1000.0, 1000.0, 1000.0])

    # === LEFT HAND ===
    tf_hand = dart.math.Isometry3()
    tf_hand.set_rotation(dart.math.exp_map_rot(np.array([0.0, 0.0, 90.0 * np.pi / 180.0])))
    tf_hand.set_translation(np.array([0.0009, 0.1254, 0.012]))

    l_hand_node = atlas.get_body_node("l_hand")
    l_hand = l_hand_node.create_end_effector("l_hand")
    l_hand.setDefaultRelativeTransform(tf_hand, True)

    # Create simple frame target (not interactive)
    lh_target = dart.dynamics.SimpleFrame(dart.dynamics.Frame.World(), "lh_target")

    l_hand_ik = l_hand.get_ik(create_if_null=True)
    l_hand_ik.set_target(lh_target)
    l_hand_ik.use_whole_body()

    weights = np.ones(atlas.get_num_dofs())
    weights[0:6] = 0.1
    l_hand_ik.get_gradient_method().set_component_weights(weights)
    l_hand_ik.get_error_method().set_linear_bounds(-linear_bounds, linear_bounds)
    l_hand_ik.get_error_method().set_angular_bounds(-angular_bounds, angular_bounds)

    # === RIGHT HAND ===
    tf_hand_r = dart.math.Isometry3()
    tf_hand_r.set_rotation(dart.math.exp_map_rot(np.array([0.0, 0.0, -90.0 * np.pi / 180.0])))
    tf_hand_r.set_translation(np.array([0.0009, -0.1254, 0.012]))

    r_hand_node = atlas.get_body_node("r_hand")
    r_hand = r_hand_node.create_end_effector("r_hand")
    r_hand.setDefaultRelativeTransform(tf_hand_r, True)

    rh_target = dart.dynamics.SimpleFrame(dart.dynamics.Frame.World(), "rh_target")

    r_hand_ik = r_hand.get_ik(create_if_null=True)
    r_hand_ik.set_target(rh_target)
    r_hand_ik.use_whole_body()
    r_hand_ik.get_gradient_method().set_component_weights(weights)
    r_hand_ik.get_error_method().set_linear_bounds(-linear_bounds, linear_bounds)
    r_hand_ik.get_error_method().set_angular_bounds(-angular_bounds, angular_bounds)

    # === FEET ===
    support_geometry = [
        np.array([-0.03 - 0.186, -0.03, 0.0]),
        np.array([0.10 - 0.186, -0.03, 0.0]),
        np.array([0.10 - 0.186, 0.03, 0.0]),
        np.array([-0.03 - 0.186, 0.03, 0.0])
    ]

    tf_foot = np.eye(4)
    tf_foot[:3, 3] = [0.186, 0.0, -0.08]

    foot_linear_bounds = np.array([1000.0, 1000.0, 1e-8])
    foot_angular_bounds = np.array([1e-8, 1e-8, 1000.0])

    # LEFT FOOT
    l_foot_node = atlas.get_body_node("l_foot")
    l_foot = l_foot_node.create_end_effector("l_foot")
    l_foot.setRelativeTransform(tf_foot)

    lf_target = dart.dynamics.SimpleFrame(dart.dynamics.Frame.World(), "lf_target")

    l_foot_ik = l_foot.get_ik(create_if_null=True)
    l_foot_ik.set_target(lf_target)
    l_foot_ik.set_hierarchy_level(1)
    l_foot_ik.get_error_method().set_linear_bounds(-foot_linear_bounds, foot_linear_bounds)
    l_foot_ik.get_error_method().set_angular_bounds(-foot_angular_bounds, foot_angular_bounds)

    l_foot_support = l_foot.get_support(create_if_null=True)
    l_foot_support.setGeometry(support_geometry)
    l_foot_support.setActive()

    # RIGHT FOOT
    r_foot_node = atlas.get_body_node("r_foot")
    r_foot = r_foot_node.create_end_effector("r_foot")
    r_foot.setRelativeTransform(tf_foot)

    rf_target = dart.dynamics.SimpleFrame(dart.dynamics.Frame.World(), "rf_target")

    r_foot_ik = r_foot.get_ik(create_if_null=True)
    r_foot_ik.set_target(rf_target)
    r_foot_ik.set_hierarchy_level(1)
    r_foot_ik.get_error_method().set_linear_bounds(-foot_linear_bounds, foot_linear_bounds)
    r_foot_ik.get_error_method().set_angular_bounds(-foot_angular_bounds, foot_angular_bounds)

    r_foot_support = r_foot.get_support(create_if_null=True)
    r_foot_support.setGeometry(support_geometry)
    r_foot_support.setActive()

    # Set foot targets to current positions
    height_change = -r_foot.getWorldTransform()[2, 3]
    atlas.get_dof(5).set_position(height_change)

    l_foot_ik.get_target().set_transform(l_foot.getTransform())
    r_foot_ik.get_target().set_transform(r_foot.getTransform())

    return {
        "l_hand": lh_target,
        "r_hand": rh_target,
        "l_foot": lf_target,
        "r_foot": rf_target
    }


def main():
    """Main function to run simplified atlas puppet."""
    try:
        print("=== Atlas Puppet (Simplified) - IK Demo ===\n")

        # Create world
        world = dart.simulation.World()
        print(f"Created world: {world.get_name()}")

        # Load Atlas
        print("\nLoading Atlas robot...")
        atlas = create_atlas()
        world.add_skeleton(atlas)
        print(f"✓ Loaded {atlas.get_name()} ({atlas.get_num_dofs()} DOFs)")

        # Set initial configuration
        print("\nSetting initial configuration...")
        setup_start_configuration(atlas)
        print("✓ Configuration set")

        # Setup end effectors with IK
        print("\nSetting up end effectors and IK targets...")
        ik_targets = setup_end_effectors(atlas)
        print("✓ Created 4 end effectors (hands + feet) with IK targets")

        # Setup whole-body solver
        print("\nConfiguring whole-body IK solver...")
        setup_whole_body_solver(atlas)

        # Create GUI
        print("\nInitializing GUI...")
        viewer = dart.gui.osg.Viewer()
        viewer.allow_simulation(False)

        world_node = dart.gui.osg.WorldNode(world)
        viewer.add_world_node(world_node)

        viewer.set_up_view_in_window(0, 0, 1280, 960)
        viewer.set_camera_home_position(
            eye=np.array([5.34, 3.00, 2.41]),
            center=np.array([0.00, 0.00, 1.00]),
            up=np.array([-0.20, -0.08, 0.98])
        )

        print("\n" + "="*60)
        print("ATLAS IK VISUALIZATION")
        print("="*60)
        print("  Mouse Wheel:    Zoom in/out")
        print("  Right Mouse:    Rotate camera")
        print("  Middle Mouse:   Pan camera")
        print("="*60)
        print("\nStarting GUI... (close window to exit)")
        print("Note: This version doesn't support interactive dragging.")
        print("For full interactive IK, use the C++ atlas_puppet example.")
        print("="*60 + "\n")

        # Run the viewer
        print(">>> Calling viewer.run()...")
        viewer.run()
        print(">>> viewer.run() returned")

        print("\n✓ Example completed!")

    except Exception as e:
        print(f"\n❌ ERROR: {e}")
        import traceback
        traceback.print_exc()
        raise


if __name__ == "__main__":
    main()

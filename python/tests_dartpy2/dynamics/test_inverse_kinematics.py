# Copyright (c) 2011-2025, The DART development contributors

import math
import numpy as np
import pytest


def test_inverse_kinematics_basic():
    """Test basic InverseKinematics functionality with end effector."""
    from dartpy2.dynamics import Skeleton, SimpleFrame
    from dartpy2.io import DartLoader

    # Load a robot skeleton
    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert skel is not None

    # Get the last body node
    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)

    # Create end effector
    ee = last_body.create_end_effector("ee")
    assert ee is not None

    # Get IK module from end effector
    ik = ee.get_ik()
    assert ik is not None


def test_inverse_kinematics_active_state():
    """Test IK active state management."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Test active state
    ik.set_active(True)
    assert ik.is_active()

    ik.set_active(False)
    assert not ik.is_active()


def test_inverse_kinematics_hierarchy_level():
    """Test IK hierarchy level get/set."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Test hierarchy level
    ik.set_hierarchy_level(1)
    assert ik.get_hierarchy_level() == 1

    ik.set_hierarchy_level(0)
    assert ik.get_hierarchy_level() == 0


def test_inverse_kinematics_chain_vs_whole_body():
    """Test chain vs whole body IK modes."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Test use_chain
    ik.use_chain()
    # Chain mode activated (no direct way to verify)

    # Test use_whole_body
    ik.use_whole_body()
    # Whole body mode activated (no direct way to verify)


def test_inverse_kinematics_target():
    """Test IK target frame management."""
    from dartpy2.dynamics import SimpleFrame
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Create a target frame
    target = SimpleFrame.create(None, "target")

    # Set target
    ik.set_target(target)

    # Get target
    retrieved_target = ik.get_target()
    assert retrieved_target is not None


def test_inverse_kinematics_error_method():
    """Test IK error method access."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Get error method
    error_method = ik.get_error_method()
    assert error_method is not None

    # Test bounds
    linear_bounds = error_method.get_linear_bounds()
    assert linear_bounds is not None
    assert len(linear_bounds) == 2  # (lower, upper)

    angular_bounds = error_method.get_angular_bounds()
    assert angular_bounds is not None
    assert len(angular_bounds) == 2  # (lower, upper)


def test_inverse_kinematics_gradient_method():
    """Test IK gradient method access."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Get gradient method
    gradient_method = ik.get_gradient_method()
    assert gradient_method is not None

    # Test setting component weights
    num_dofs = skel.get_num_dofs()
    weights = np.ones(num_dofs)
    gradient_method.set_component_weights(weights)


def test_inverse_kinematics_solve_and_apply():
    """Test IK solve_and_apply method."""
    from dartpy2.dynamics import SimpleFrame
    from dartpy2.io import DartLoader
    import numpy as np

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Create target
    target = SimpleFrame.create(None, "target")

    # Set a simple target transform (identity for now)
    target_transform = np.eye(4)
    target_transform[2, 3] = 0.5  # Translate in Z
    target.set_transform(target_transform)

    ik.set_target(target)
    ik.set_active(True)

    # Try to solve (may not converge, but should not crash)
    result = ik.solve_and_apply(allow_incomplete_result=True)
    assert isinstance(result, bool)


def test_inverse_kinematics_clear_caches():
    """Test IK cache clearing."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    skel = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    last_body = skel.get_body_node(skel.get_num_body_nodes() - 1)
    ee = last_body.create_end_effector("ee")
    ik = ee.get_ik()

    # Clear caches (should not crash)
    ik.clear_caches()


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

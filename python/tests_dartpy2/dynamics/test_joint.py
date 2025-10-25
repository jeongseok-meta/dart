# Copyright (c) 2011-2025, The DART development contributors

import math
import numpy as np
import pytest


def test_joint_basic():
    """Test basic Joint functionality."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert kr5 is not None

    num_joints = kr5.get_num_joints()
    assert num_joints > 0

    # Test accessing joints by index
    for i in range(num_joints):
        joint = kr5.get_joint(i)
        assert joint is not None


def test_joint_placeholder():
    """
    Placeholder test for Joint class methods.

    Note: Many Joint methods are not yet bound in dartpy2.
    This test is a placeholder that can be expanded once more bindings are added.
    """
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    # Access first joint (should be root joint)
    joint = kr5.get_joint(0)
    assert joint is not None

    # TODO: Add tests for joint methods once bound:
    # - get_name(), set_name()
    # - get_num_dofs()
    # - get_positions(), set_positions()
    # - get_velocities(), set_velocities()
    # - get_relative_transform()
    # - get_relative_jacobian()
    # - set_transform_from_parent_body_node()
    # - set_transform_from_child_body_node()


def test_free_joint_basic():
    """Test FreeJoint functionality."""
    from dartpy2.dynamics import Skeleton

    # Create skeleton with free joint
    skel = Skeleton.create("free_joint_test")
    assert skel is not None

    # TODO: Add FreeJoint creation and testing once API is available
    # Currently create_free_joint_and_body_node_pair is not bound
    # joint, body = skel.create_free_joint_and_body_node_pair()
    # assert joint is not None
    # assert body is not None
    # assert joint.get_num_dofs() == 6


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

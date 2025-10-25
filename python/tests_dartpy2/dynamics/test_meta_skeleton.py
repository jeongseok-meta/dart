# Copyright (c) 2011-2025, The DART development contributors

import pytest


def test_meta_skeleton_basic():
    """Test basic MetaSkeleton functionality through Skeleton."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert kr5 is not None

    # MetaSkeleton methods are available through Skeleton
    positions = kr5.get_positions()
    assert positions is not None
    assert len(positions) > 0

    num_joints = kr5.get_num_joints()
    assert num_joints > 0

    # Access root joint
    root_joint = kr5.get_joint(0)
    assert root_joint is not None


def test_meta_skeleton_body_node_access():
    """Test body node access methods."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    num_body_nodes = kr5.get_num_body_nodes()
    assert num_body_nodes > 0

    # Get body node by index
    root_body = kr5.get_body_node(0)
    assert root_body is not None


def test_meta_skeleton_placeholder():
    """
    Placeholder test for MetaSkeleton and Chain classes.

    Note: Chain class and many MetaSkeleton methods are not yet bound in dartpy2.
    This test is a placeholder that can be expanded once bindings are added.
    """
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    # TODO: Add tests for Chain class once bound:
    # from dartpy2.dynamics import Chain
    # shoulder = kr5.get_body_node_by_name("shoulder")
    # elbow = kr5.get_body_node_by_name("elbow")
    # chain = Chain(shoulder, elbow, False, "midchain")
    # assert chain is not None
    # assert chain.get_num_body_nodes() == 2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

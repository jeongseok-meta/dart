# Copyright (c) 2011-2025, The DART development contributors

import numpy as np
import pytest


def test_body_node_basic():
    """Test basic BodyNode functionality."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert kr5 is not None

    num_body_nodes = kr5.get_num_body_nodes()
    assert num_body_nodes > 0

    # Test accessing body nodes by index
    for i in range(num_body_nodes):
        body = kr5.get_body_node(i)
        assert body is not None
        assert isinstance(body.get_name(), str)
        assert len(body.get_name()) > 0


def test_body_node_name():
    """Test BodyNode name get/set operations."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    body = kr5.get_body_node(0)
    original_name = body.get_name()
    assert original_name is not None

    # Set new name
    new_name = "test_body"
    body.set_name(new_name)
    assert body.get_name() == new_name

    # Restore original name
    body.set_name(original_name)
    assert body.get_name() == original_name


def test_body_node_world_transform():
    """Test getting world transform from BodyNode."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    body = kr5.get_body_node(0)
    transform = body.get_world_transform()
    assert transform is not None
    # Transform should be 4x4 matrix
    assert transform.shape == (4, 4)


def test_body_node_get_child_methods():
    """Test getting child body nodes."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")
    assert kr5 is not None

    # Get root body node
    root_body = kr5.get_body_node(0)
    assert root_body is not None

    # Check number of children
    num_children = root_body.get_num_child_body_nodes()
    assert num_children >= 0


def test_body_node_repr():
    """Test BodyNode string representation."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    body = kr5.get_body_node(0)
    repr_str = repr(body)

    assert "BodyNode" in repr_str
    assert body.get_name() in repr_str


def test_body_node_create_end_effector():
    """Test creating end effector on a body node."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    kr5 = loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    # Get last body node to use as end effector
    last_body = kr5.get_body_node(kr5.get_num_body_nodes() - 1)

    # Create end effector
    ee = last_body.create_end_effector("test_ee")
    assert ee is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

import pytest


def test_skeleton_creation():
    """Test basic Skeleton creation."""
    from dartpy2.dynamics import Skeleton

    # Note: Using Skeleton.create() due to C++ factory pattern with shared_ptr.
    # See skeleton.cpp for detailed explanation.
    skel = Skeleton.create()
    assert skel is not None
    assert len(skel.get_name()) > 0  # Should have auto-generated name


def test_skeleton_with_name():
    """Test Skeleton creation with name."""
    from dartpy2.dynamics import Skeleton

    skel = Skeleton.create("robot")
    assert skel.get_name() == "robot"


def test_skeleton_name_operations():
    """Test Skeleton name get/set."""
    from dartpy2.dynamics import Skeleton

    skel = Skeleton.create()
    skel.set_name("my_robot")
    assert skel.get_name() == "my_robot"


def test_skeleton_empty_counts():
    """Test empty Skeleton has zero body nodes and DOFs."""
    from dartpy2.dynamics import Skeleton

    skel = Skeleton.create("empty")
    assert skel.get_num_body_nodes() == 0
    assert skel.get_num_dofs() == 0


def test_skeleton_repr():
    """Test Skeleton __repr__ method."""
    from dartpy2.dynamics import Skeleton

    skel = Skeleton.create("test_robot")
    repr_str = repr(skel)

    assert "Skeleton" in repr_str
    assert "test_robot" in repr_str
    assert "num_body_nodes" in repr_str


def test_skeleton_get_positions():
    """Test getting skeleton positions."""
    import dartpy2 as dart

    # Create a skeleton and load URDF
    dart_loader = dart.io.DartLoader()
    kr5 = dart_loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    # Get positions
    positions = kr5.get_positions()
    assert positions is not None
    assert len(positions) == kr5.get_num_dofs()


def test_skeleton_set_positions():
    """Test setting skeleton positions."""
    import numpy as np
    import dartpy2 as dart

    # Create a skeleton and load URDF
    dart_loader = dart.io.DartLoader()
    kr5 = dart_loader.parse_skeleton("dart://sample/urdf/KR5/KR5 sixx R650.urdf")

    # Set positions
    num_dofs = kr5.get_num_dofs()
    new_positions = np.ones(num_dofs) * 0.5
    kr5.set_positions(new_positions)

    # Verify positions were set
    positions = kr5.get_positions()
    assert np.allclose(positions, new_positions)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

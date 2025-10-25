import pytest


def test_dof_access_by_index():
    """Test accessing DOFs by index from a loaded skeleton."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    # Atlas should have many DOFs
    num_dofs = atlas.get_num_dofs()
    assert num_dofs > 0

    # Access first DOF
    dof0 = atlas.get_dof(0)
    assert dof0 is not None
    assert dof0.get_index_in_skeleton() == 0


def test_dof_access_by_name():
    """Test accessing DOFs by name from a loaded skeleton."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    # Try to get a known joint - atlas has joints like "l_leg_hpy"
    dof = atlas.get_dof("l_leg_hpy")
    assert dof is not None
    assert dof.get_name() == "l_leg_hpy"


def test_dof_position_manipulation():
    """Test setting and getting DOF positions."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    # Get a specific DOF
    dof = atlas.get_dof("l_leg_hpy")

    # Get initial position
    initial_pos = dof.get_position()

    # Set a new position
    new_pos = 0.5
    dof.set_position(new_pos)

    # Verify it was set
    assert abs(dof.get_position() - new_pos) < 1e-10

    # Reset and verify
    dof.set_position(initial_pos)
    assert abs(dof.get_position() - initial_pos) < 1e-10


def test_dof_velocity():
    """Test setting and getting DOF velocity."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    dof = atlas.get_dof(0)

    # Set velocity
    vel = 1.5
    dof.set_velocity(vel)
    assert abs(dof.get_velocity() - vel) < 1e-10


def test_dof_position_limits():
    """Test getting DOF position limits."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    dof = atlas.get_dof("l_leg_hpy")

    # Get limits
    limits = dof.get_position_limits()
    assert isinstance(limits, tuple)
    assert len(limits) == 2

    lower = dof.get_position_lower_limit()
    upper = dof.get_position_upper_limit()

    assert limits[0] == lower
    assert limits[1] == upper
    assert lower < upper


def test_dof_repr():
    """Test DOF string representation."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    dof = atlas.get_dof("l_leg_hpy")
    repr_str = repr(dof)

    assert "DegreeOfFreedom" in repr_str
    assert "l_leg_hpy" in repr_str
    assert "position" in repr_str


def test_multiple_dofs_independent():
    """Test that DOF modifications are independent."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    dof1 = atlas.get_dof("l_leg_hpy")
    dof2 = atlas.get_dof("r_leg_hpy")

    # Set different positions
    dof1.set_position(0.3)
    dof2.set_position(0.7)

    # Verify they're different
    assert abs(dof1.get_position() - 0.3) < 1e-10
    assert abs(dof2.get_position() - 0.7) < 1e-10


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

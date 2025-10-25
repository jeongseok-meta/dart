import pytest


def test_dart_loader_creation():
    """Test DartLoader creation."""
    from dartpy2.io import DartLoader

    loader = DartLoader()
    assert loader is not None


def test_load_atlas_urdf():
    """Test loading Atlas URDF from dart:// URI."""
    from dartpy2.io import DartLoader

    loader = DartLoader()

    # Load the atlas robot from embedded resources
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    assert atlas is not None
    assert atlas.get_name() == "drc_skeleton"  # Name from the URDF file
    assert atlas.get_num_dofs() > 0
    assert atlas.get_num_body_nodes() > 0


def test_add_skeleton_to_world():
    """Test adding a loaded skeleton to a world."""
    from dartpy2.io import DartLoader
    from dartpy2.simulation import World

    # Create world
    world = World()
    assert world.get_num_skeletons() == 0

    # Load atlas
    loader = DartLoader()
    atlas = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")

    # Add to world
    world.add_skeleton(atlas)
    assert world.get_num_skeletons() == 1

    # Can add multiple skeletons
    atlas2 = loader.parse_skeleton("dart://sample/sdf/atlas/atlas_v3_no_head.urdf")
    world.add_skeleton(atlas2)
    assert world.get_num_skeletons() == 2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

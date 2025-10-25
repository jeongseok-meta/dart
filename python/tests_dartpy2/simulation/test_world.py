import pytest
import numpy as np


def test_world_creation():
    """Test World creation."""
    from dartpy2.simulation import World

    world = World()
    assert world is not None


def test_world_with_name():
    """Test World creation with name."""
    from dartpy2.simulation import World

    world = World("MyWorld")
    assert world.get_name() == "MyWorld"


def test_world_name():
    """Test World name operations."""
    from dartpy2.simulation import World

    world = World()
    world.set_name("NewName")
    assert world.get_name() == "NewName"


def test_world_gravity():
    """Test World gravity methods with multiple input types.

    Design decision: Accept lists, tuples, numpy arrays, and PyTorch tensors for flexibility.
    - Tuples: Recommended style (immutable, Genesis-like)
    - Lists: Most Pythonic for general use
    - NumPy arrays: Standard for scientific computing
    - PyTorch tensors: Deep learning integration
    """
    import dartpy2.simulation

    world = dartpy2.simulation.World()
    gravity = world.get_gravity()
    assert len(gravity) == 3
    assert gravity[2] < 0

    # Test with tuple (recommended style, like Genesis simulator)
    world.set_gravity((0.0, 0.0, -9.81))
    gravity = world.get_gravity()
    assert np.allclose(gravity, [0.0, 0.0, -9.81])

    # Test with Python list (Pythonic!)
    world.set_gravity([1.0, 2.0, 3.0])
    gravity = world.get_gravity()
    assert np.allclose(gravity, [1.0, 2.0, 3.0])

    # Test with numpy array (scientific computing standard)
    world.set_gravity(np.array([4.0, 5.0, 6.0]))
    gravity = world.get_gravity()
    assert np.allclose(gravity, [4.0, 5.0, 6.0])

    # Test with integer list (should auto-convert to float)
    world.set_gravity([7, 8, 9])
    gravity = world.get_gravity()
    assert np.allclose(gravity, [7.0, 8.0, 9.0])

    # Test with mixed int/float tuple
    world.set_gravity((10, 11.5, 12))
    gravity = world.get_gravity()
    assert np.allclose(gravity, [10.0, 11.5, 12.0])

    # Test with PyTorch tensor (deep learning integration)
    try:
        import torch
        world.set_gravity(torch.tensor([0.0, -5.0, -10.0]))
        gravity = world.get_gravity()
        assert np.allclose(gravity, [0.0, -5.0, -10.0])
    except ImportError:
        pytest.skip("PyTorch not available")


def test_world_gravity_errors():
    """Test World gravity error handling."""
    import dartpy2.simulation

    world = dartpy2.simulation.World()

    # Wrong length
    with pytest.raises(RuntimeError, match="Expected sequence of length 3"):
        world.set_gravity([1.0, 2.0])  # Too short

    with pytest.raises(RuntimeError, match="Expected sequence of length 3"):
        world.set_gravity([1.0, 2.0, 3.0, 4.0])  # Too long

    # Invalid types
    with pytest.raises(RuntimeError, match="Expected a sequence"):
        world.set_gravity(42)  # Not a sequence

    # String is technically a sequence but wrong length (7 chars)
    with pytest.raises(RuntimeError, match="Expected sequence of length 3"):
        world.set_gravity("invalid")


def test_world_time_step():
    """Test World time step operations."""
    from dartpy2.simulation import World

    world = World()

    # Set time step
    world.set_time_step(0.001)
    assert abs(world.get_time_step() - 0.001) < 1e-10


def test_world_time():
    """Test World time operations."""
    from dartpy2.simulation import World

    world = World()

    # Initial time should be 0
    assert world.get_time() == 0.0

    # Set time
    world.set_time(5.0)
    assert world.get_time() == 5.0


def test_world_reset():
    """Test World reset."""
    from dartpy2.simulation import World

    world = World()
    world.set_time(10.0)
    world.reset()
    assert world.get_time() == 0.0


def test_world_step():
    """Test World step operations."""
    from dartpy2.simulation import World

    world = World()
    world.set_time_step(0.001)

    initial_time = world.get_time()
    world.step()

    # Time should advance by one time step
    assert abs(world.get_time() - (initial_time + 0.001)) < 1e-10

    # Step with reset_command parameter
    world.step(False)
    assert abs(world.get_time() - (initial_time + 0.002)) < 1e-10


def test_world_sim_frames():
    """Test World simulation frames counter."""
    from dartpy2.simulation import World

    world = World()

    initial_frames = world.get_sim_frames()
    world.step()

    # Sim frames should increment
    assert world.get_sim_frames() == initial_frames + 1


def test_world_num_skeletons():
    """Test World skeleton count."""
    from dartpy2.simulation import World

    world = World()

    # Initially no skeletons
    assert world.get_num_skeletons() == 0


def test_world_repr():
    """Test World __repr__ method."""
    from dartpy2.simulation import World

    world = World("TestWorld")
    repr_str = repr(world)

    assert "World" in repr_str
    assert "TestWorld" in repr_str
    assert "num_skeletons" in repr_str
    assert "time" in repr_str


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

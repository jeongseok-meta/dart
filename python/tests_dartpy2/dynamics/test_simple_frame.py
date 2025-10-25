# Copyright (c) 2011-2025, The DART development contributors

import numpy as np
import pytest


def test_simple_frame_creation():
    """Test SimpleFrame creation."""
    from dartpy2.dynamics import SimpleFrame

    # Create a simple frame with no parent (world frame)
    frame1 = SimpleFrame.create(None)
    assert frame1 is not None


def test_simple_frame_with_name():
    """Test SimpleFrame creation with name."""
    from dartpy2.dynamics import SimpleFrame

    frame = SimpleFrame.create(None, "test_frame")
    assert frame is not None
    assert frame.get_name() == "test_frame"


def test_simple_frame_name_operations():
    """Test SimpleFrame name get/set."""
    from dartpy2.dynamics import SimpleFrame

    frame = SimpleFrame.create(None)

    # Set new name
    frame.set_name("my_frame")
    assert frame.get_name() == "my_frame"


def test_simple_frame_transform():
    """Test SimpleFrame transform get/set."""
    from dartpy2.dynamics import SimpleFrame

    frame = SimpleFrame.create(None)

    # Get initial transform
    transform = frame.get_transform()
    assert transform is not None
    assert transform.shape == (4, 4)

    # Initial transform should be identity
    assert np.allclose(transform[:3, 3], [0, 0, 0])

    # Set new transform
    new_transform = np.eye(4)
    new_transform[0, 3] = 1.0
    new_transform[1, 3] = 2.0
    new_transform[2, 3] = 3.0

    frame.set_transform(new_transform)

    # Verify transform was set
    retrieved_transform = frame.get_transform()
    assert np.allclose(retrieved_transform[:3, 3], [1, 2, 3])


def test_simple_frame_relative_transform():
    """Test SimpleFrame relative transform."""
    from dartpy2.dynamics import SimpleFrame

    # Create parent frame
    parent_frame = SimpleFrame.create(None, "parent")
    parent_transform = np.eye(4)
    parent_transform[0, 3] = 1.0
    parent_transform[1, 3] = 2.0
    parent_transform[2, 3] = 3.0
    parent_frame.set_transform(parent_transform)

    # Create child frame (for now, test with None parent as hierarchy is not fully bound)
    child_frame = SimpleFrame.create(None, "child")

    # Set relative transform
    relative_transform = np.eye(4)
    relative_transform[0, 3] = 0.5
    relative_transform[1, 3] = 0.5
    relative_transform[2, 3] = 0.5

    child_frame.set_relative_transform(relative_transform)

    # Verify transform
    child_transform = child_frame.get_transform()
    assert child_transform is not None


def test_simple_frame_repr():
    """Test SimpleFrame string representation."""
    from dartpy2.dynamics import SimpleFrame

    frame = SimpleFrame.create(None, "test_frame")
    repr_str = repr(frame)

    assert "SimpleFrame" in repr_str
    assert "test_frame" in repr_str


def test_simple_frame_multiple_instances():
    """Test creating multiple SimpleFrame instances."""
    from dartpy2.dynamics import SimpleFrame

    frame1 = SimpleFrame.create(None, "frame1")
    frame2 = SimpleFrame.create(None, "frame2")

    assert frame1.get_name() == "frame1"
    assert frame2.get_name() == "frame2"

    # Set different transforms
    transform1 = np.eye(4)
    transform1[0, 3] = 1.0

    transform2 = np.eye(4)
    transform2[0, 3] = 2.0

    frame1.set_transform(transform1)
    frame2.set_transform(transform2)

    # Verify they're independent
    t1 = frame1.get_transform()
    t2 = frame2.get_transform()

    assert np.isclose(t1[0, 3], 1.0)
    assert np.isclose(t2[0, 3], 2.0)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

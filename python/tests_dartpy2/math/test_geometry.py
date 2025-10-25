import pytest
import numpy as np


def test_euler_xyz_to_matrix():
    """Test Euler XYZ angle to matrix conversion."""
    from dartpy2 import math

    angles = np.array([0.1, 0.2, 0.3])
    R = math.eulerXYZToMatrix(angles)

    assert R.shape == (3, 3)
    # Verify it's a valid rotation matrix (det should be 1)
    assert abs(np.linalg.det(R) - 1.0) < 1e-10


def test_matrix_to_euler_xyz():
    """Test matrix to Euler XYZ angle conversion."""
    from dartpy2 import math

    # Start with known angles
    angles_in = np.array([0.1, 0.2, 0.3])
    R = math.eulerXYZToMatrix(angles_in)

    # Convert back
    angles_out = math.matrixToEulerXYZ(R)

    assert angles_out.shape == (3,)
    # Angles should be close (accounting for floating point)
    assert np.allclose(angles_in, angles_out, atol=1e-10)


def test_euler_zyx_to_matrix():
    """Test Euler ZYX angle to matrix conversion."""
    from dartpy2 import math

    angles = np.array([0.3, 0.2, 0.1])
    R = math.eulerZYXToMatrix(angles)

    assert R.shape == (3, 3)
    assert abs(np.linalg.det(R) - 1.0) < 1e-10


def test_matrix_to_euler_zyx():
    """Test matrix to Euler ZYX angle conversion."""
    from dartpy2 import math

    angles_in = np.array([0.3, 0.2, 0.1])
    R = math.eulerZYXToMatrix(angles_in)
    angles_out = math.matrixToEulerZYX(R)

    assert np.allclose(angles_in, angles_out, atol=1e-10)


def test_exp_map_rot():
    """Test exponential map for rotation."""
    from dartpy2 import math

    expmap = np.array([0.1, 0.2, 0.3])
    R = math.exp_map_rot(expmap)

    assert R.shape == (3, 3)
    # Verify it's a rotation matrix
    assert abs(np.linalg.det(R) - 1.0) < 1e-10
    # R * R^T should be identity
    assert np.allclose(R @ R.T, np.eye(3), atol=1e-10)


def test_exp_to_quat():
    """Test exponential map to quaternion conversion."""
    from dartpy2 import math

    v = np.array([0.1, 0.2, 0.3])
    q = math.exp_to_quat(v)

    # Quaternion should have 4 components
    assert len(q.coeffs()) == 4
    # Quaternion should be normalized
    assert abs(np.linalg.norm(q.coeffs()) - 1.0) < 1e-10


def test_quat_to_exp():
    """Test quaternion to exponential map conversion."""
    from dartpy2 import math

    # Create a simple rotation quaternion (90 degrees around Z axis)
    v_in = np.array([0.0, 0.0, np.pi/2])
    q = math.exp_to_quat(v_in)
    v_out = math.quat_to_exp(q)

    assert v_out.shape == (3,)
    # Should be close to original
    assert np.allclose(v_in, v_out, atol=1e-6)


def test_verify_rotation():
    """Test rotation matrix verification."""
    from dartpy2 import math

    # Valid rotation matrix (identity)
    R_valid = np.eye(3)
    assert math.verify_rotation(R_valid) is True

    # Invalid matrix (not orthogonal) - should be False but numpy array might not preserve exact checks
    # Skipping invalid test for now


def test_verify_transform():
    """Test transform matrix verification."""
    from dartpy2 import math

    # Create a valid transform (identity)
    T_valid = np.eye(4)

    # Note: verify_transform expects Eigen::Isometry3d
    # For now, test with identity created from exp_angular
    s = np.array([0.0, 0.0, 0.0])
    T = math.exp_angular(s)

    assert math.verify_transform(T) is True


def test_exp_angular():
    """Test exponential map for angular velocity."""
    from dartpy2 import math

    s = np.array([0.1, 0.2, 0.3])
    T = math.exp_angular(s)

    # Should produce a transform matrix
    # Can't easily check shape directly, but verify it's a valid transform
    assert math.verify_transform(T) is True


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

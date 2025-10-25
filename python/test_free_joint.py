#!/usr/bin/env python3
"""
Test FreeJoint conversion functions
"""

import numpy as np
import sys
sys.path.insert(0, "build/default/cpp/python/dartpy2")
import dartpy2 as dart

def test_free_joint_conversions():
    print("="*60)
    print("Testing FreeJoint Conversion Functions")
    print("="*60)

    # Create a test transform using Isometry3
    transform = dart.math.Isometry3()
    rotation = dart.math.exp_map_rot(np.array([0.1, 0.2, 0.3]))
    transform.set_rotation(rotation)
    transform.set_translation(np.array([1.0, 2.0, 3.0]))

    print("\nOriginal Transform:")
    print(transform.matrix())

    # Convert to FreeJoint positions
    positions = dart.dynamics.free_joint_convert_to_positions(transform)
    print(f"\nConverted to positions (6D): {positions}")

    # Convert back to transform
    recovered_transform = dart.dynamics.free_joint_convert_to_transform(positions)
    print("\nRecovered Transform:")
    print(recovered_transform.matrix())

    # Check if they match
    diff = np.linalg.norm(transform.matrix() - recovered_transform.matrix())
    print(f"\nTransform difference (should be ~0): {diff}")

    if diff < 1e-10:
        print("\n✅ FreeJoint conversion test PASSED!")
        return True
    else:
        print(f"\n❌ FreeJoint conversion test FAILED! Difference: {diff}")
        return False

if __name__ == "__main__":
    success = test_free_joint_conversions()
    sys.exit(0 if success else 1)

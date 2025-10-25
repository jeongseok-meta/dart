#!/usr/bin/env python3
"""
Test optimizer module - ModularFunction with Python callables
"""

import numpy as np

def test_optimizer():
    print("="*60)
    print("Testing optimizer.ModularFunction")
    print("="*60)

    # Import dartpy2
    import sys
    sys.path.insert(0, "build/default/cpp/python/dartpy2")
    import dartpy2 as dart

    # Check if optimizer module exists
    try:
        optimizer = dart.optimizer
        print("✓ dart.optimizer module found")
    except AttributeError as e:
        print(f"✗ ERROR: {e}")
        return False

    # Create a ModularFunction
    try:
        func = dart.optimizer.ModularFunction("test_function")
        print(f"✓ Created ModularFunction: {func.get_name()}")
    except Exception as e:
        print(f"✗ ERROR creating ModularFunction: {e}")
        return False

    # Define a simple quadratic cost function
    def cost(x):
        # Cost = 0.5 * ||x||^2
        return 0.5 * np.dot(x, x)

    # Define gradient function
    def gradient(x, grad):
        # Gradient = x
        grad[:] = x

    # Set the functions
    try:
        func.set_cost_function(cost)
        print("✓ Set cost function")

        func.set_gradient_function(gradient)
        print("✓ Set gradient function")
    except Exception as e:
        print(f"✗ ERROR setting functions: {e}")
        return False

    # Test evaluation
    x = np.array([1.0, 2.0, 3.0])
    try:
        cost_val = func.eval(x)
        expected_cost = 0.5 * np.dot(x, x)  # 0.5 * (1 + 4 + 9) = 7.0
        print(f"✓ Cost evaluation: {cost_val} (expected: {expected_cost})")
        assert abs(cost_val - expected_cost) < 1e-10, f"Cost mismatch: {cost_val} vs {expected_cost}"
    except Exception as e:
        print(f"✗ ERROR evaluating cost: {e}")
        return False

    # Test gradient
    grad = np.zeros(3)
    try:
        func.eval_gradient(x, grad)
        print(f"✓ Gradient evaluation: {grad}")
        print(f"  Expected: {x}")
        assert np.allclose(grad, x), f"Gradient mismatch: {grad} vs {x}"
    except Exception as e:
        print(f"✗ ERROR evaluating gradient: {e}")
        return False

    print("\n" + "="*60)
    print("✅ OPTIMIZER MODULE TEST PASSED!")
    print("="*60)
    print("\nNext: Use this in RelaxedPosture for atlas_puppet")
    print("="*60)

    return True

if __name__ == "__main__":
    import sys
    success = test_optimizer()
    sys.exit(0 if success else 1)

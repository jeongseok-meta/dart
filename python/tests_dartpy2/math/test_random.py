import pytest


def test_random_class_exists():
    """Test that Random class exists."""
    from dartpy2.math import Random

    assert Random is not None


def test_random_creation():
    """Test Random instance creation."""
    from dartpy2.math import Random

    rng = Random()
    assert rng is not None


def test_random_seed():
    """Test Random seed operations."""
    from dartpy2.math import Random

    Random.set_seed(42)
    seed = Random.get_seed()
    assert seed == 42


def test_random_uniform():
    """Test Random uniform distribution."""
    from dartpy2.math import Random

    Random.set_seed(42)
    value = Random.uniform(0.0, 1.0)
    assert 0.0 <= value <= 1.0


def test_random_uniform_range():
    """Test Random uniform with custom range."""
    from dartpy2.math import Random

    Random.set_seed(100)
    min_val = 10.0
    max_val = 20.0

    # Generate multiple samples to test range
    for _ in range(10):
        value = Random.uniform(min_val, max_val)
        assert min_val <= value <= max_val


def test_random_reproducibility():
    """Test that setting seed produces reproducible results."""
    from dartpy2.math import Random

    Random.set_seed(123)
    values1 = [Random.uniform(0.0, 1.0) for _ in range(5)]

    Random.set_seed(123)
    values2 = [Random.uniform(0.0, 1.0) for _ in range(5)]

    assert values1 == values2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

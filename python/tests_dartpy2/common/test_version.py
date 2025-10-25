import pytest


def test_dartpy2_import():
    """Test that dartpy2 can be imported."""
    import dartpy2


def test_dartpy2_version():
    """Test that dartpy2 has version information."""
    import dartpy2

    # dartpy2 uses the same versioning as DART library
    assert dartpy2.__version__ is not None or dartpy2.__version__ == ""


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

import pytest


def test_logging_functions():
    """Test that logging functions are available."""
    from dartpy2 import common

    assert hasattr(common, "trace")
    assert hasattr(common, "debug")
    assert hasattr(common, "info")
    assert hasattr(common, "warn")
    assert hasattr(common, "error")
    assert hasattr(common, "fatal")


def test_logging_calls():
    """Test that logging functions can be called."""
    from dartpy2 import common

    common.trace("trace log")
    common.debug("debug log")
    common.info("info log")
    common.warn("warn log")
    common.error("error log")
    common.fatal("fatal log")


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

import pytest


def test_stopwatch_creation():
    """Test Stopwatch creation."""
    from dartpy2.common import Stopwatch

    sw = Stopwatch()
    assert sw is not None
    assert sw.is_started()

    sw2 = Stopwatch(False)
    assert not sw2.is_started()


def test_stopwatch_start_stop():
    """Test Stopwatch start and stop."""
    from dartpy2.common import Stopwatch
    import time

    sw = Stopwatch(False)
    assert not sw.is_started()

    sw.start()
    assert sw.is_started()
    time.sleep(0.01)

    sw.stop()
    assert not sw.is_started()
    elapsed = sw.elapsed_ms()
    assert elapsed > 0


def test_stopwatch_reset():
    """Test Stopwatch reset."""
    from dartpy2.common import Stopwatch
    import time

    sw = Stopwatch()
    time.sleep(0.01)
    sw.reset()
    elapsed = sw.elapsed_ms()
    assert elapsed < 5


def test_stopwatch_elapsed():
    """Test Stopwatch elapsed methods."""
    from dartpy2.common import Stopwatch
    import time

    sw = Stopwatch()
    time.sleep(0.01)

    elapsed_s = sw.elapsed_s()
    elapsed_ms = sw.elapsed_ms()
    elapsed_us = sw.elapsed_us()
    elapsed_ns = sw.elapsed_ns()

    assert elapsed_s >= 0
    assert elapsed_ms >= 0
    assert elapsed_us >= 0
    assert elapsed_ns >= 0
    assert elapsed_ns > elapsed_us > elapsed_ms


def test_tic_toc():
    """Test tic and toc functions."""
    from dartpy2 import common
    import time

    common.tic()
    time.sleep(0.01)
    elapsed = common.toc(False)
    assert elapsed > 0


def test_tic_toc_units():
    """Test tic and toc with different units."""
    from dartpy2 import common
    import time

    common.tic()
    time.sleep(0.01)

    elapsed_s = common.toc_s(False)
    elapsed_ms = common.toc_ms(False)
    elapsed_us = common.toc_us(False)
    elapsed_ns = common.toc_ns(False)

    assert elapsed_s >= 0
    assert elapsed_ms >= 0
    assert elapsed_us >= 0
    assert elapsed_ns >= 0


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

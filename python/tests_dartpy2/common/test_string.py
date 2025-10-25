import pytest


def test_string_to_upper():
    """Test toUpper function."""
    from dartpy2.common import toUpper

    assert toUpper("hello") == "HELLO"
    assert toUpper("WoRlD") == "WORLD"
    assert toUpper("123abc") == "123ABC"


def test_string_to_lower():
    """Test toLower function."""
    from dartpy2.common import toLower

    assert toLower("HELLO") == "hello"
    assert toLower("WoRlD") == "world"
    assert toLower("123ABC") == "123abc"


def test_string_trim():
    """Test trim function."""
    from dartpy2.common import trim

    assert trim("  hello  ") == "hello"
    assert trim("\n\thello\t\n") == "hello"
    assert trim("hello") == "hello"
    assert trim("  hello world  ") == "hello world"


def test_string_trim_left():
    """Test trimLeft function."""
    from dartpy2.common import trimLeft

    assert trimLeft("  hello  ") == "hello  "
    assert trimLeft("\n\thello\t\n") == "hello\t\n"
    assert trimLeft("hello") == "hello"


def test_string_trim_right():
    """Test trimRight function."""
    from dartpy2.common import trimRight

    assert trimRight("  hello  ") == "  hello"
    assert trimRight("\n\thello\t\n") == "\n\thello"
    assert trimRight("hello") == "hello"


def test_string_split():
    """Test split function."""
    from dartpy2.common import split

    result = split("hello world")
    assert len(result) == 2
    assert result[0] == "hello"
    assert result[1] == "world"

    result2 = split("one,two,three", ",")
    assert len(result2) == 3
    assert result2[0] == "one"
    assert result2[1] == "two"
    assert result2[2] == "three"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

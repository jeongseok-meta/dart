import pytest


def test_uri_creation():
    """Test Uri class creation."""
    from dartpy2.common import Uri

    uri = Uri()
    assert uri is not None

    uri2 = Uri("file:///path/to/file")
    assert uri2 is not None


def test_uri_from_string():
    """Test Uri fromString method."""
    from dartpy2.common import Uri

    uri = Uri()
    assert uri.from_string("file:///path/to/file")
    assert uri.to_string() == "file:///path/to/file"


def test_uri_from_path():
    """Test Uri fromPath method."""
    from dartpy2.common import Uri

    uri = Uri()
    assert uri.from_path("/path/to/file")
    path = uri.get_path()
    assert "/path/to/file" in path


def test_uri_static_methods():
    """Test Uri static methods."""
    from dartpy2.common import Uri

    uri = Uri.create_from_string("file:///path/to/file")
    assert uri.to_string() == "file:///path/to/file"

    uri2 = Uri.create_from_path("/path/to/file")
    assert uri2.get_path() != ""


def test_uri_clear():
    """Test Uri clear method."""
    from dartpy2.common import Uri

    uri = Uri("file:///path/to/file")
    uri.clear()
    assert uri.to_string() == ""


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

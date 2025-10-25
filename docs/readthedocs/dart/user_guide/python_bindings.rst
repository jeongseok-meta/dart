Python Bindings
================

DART provides Python bindings to enable robotics and animation applications in Python. Currently, two Python binding implementations coexist:

- **dartpy** (current): Python bindings using pybind11 (experimental)
- **dartpy2** (new): Python bindings using nanobind (under active development)

dartpy2 Overview
----------------

dartpy2 is the next-generation Python binding for DART, designed to provide improved build times and runtime performance. It uses nanobind instead of pybind11, resulting in:

- **Faster build times**: nanobind generates more compact binding code
- **Better runtime performance**: Lower overhead for Python-C++ interactions
- **Smaller binary sizes**: More efficient code generation

Once dartpy2 reaches feature parity with the C++ API, it will replace the current dartpy implementation completely.

Installation
------------

dartpy2 is built alongside DART when Python bindings are enabled. If you're using pixi:

.. code-block:: bash

   pixi run build-py2

This builds the dartpy2 module. To use it in Python:

.. code-block:: bash

   pixi run py2-shell

Getting Started
---------------

Basic Usage
~~~~~~~~~~~

.. code-block:: python

   import dartpy2
   from dartpy2 import common

   # Get DART version information
   print(dartpy2.get_dart_version())
   print(f"DART {common.DART_MAJOR_VERSION}.{common.DART_MINOR_VERSION}.{common.DART_PATCH_VERSION}")

Common Module
~~~~~~~~~~~~~

The common module provides utility functions and classes:

**Logging**

.. code-block:: python

   from dartpy2 import common

   common.info("Information message")
   common.warn("Warning message")
   common.error("Error message")

**String Utilities**

.. code-block:: python

   from dartpy2 import common

   # String manipulation
   print(common.toUpper("hello"))  # "HELLO"
   print(common.toLower("WORLD"))  # "world"
   print(common.trim("  text  "))  # "text"

   # String splitting
   words = common.split("one,two,three", ",")
   # Returns: ['one', 'two', 'three']

**Stopwatch and Timing**

.. code-block:: python

   from dartpy2 import common

   # Using Stopwatch class
   sw = common.Stopwatch()
   # ... do work ...
   print(f"Elapsed: {sw.elapsedMS()}ms")

   # Using tic/toc functions
   common.tic()
   # ... do work ...
   elapsed = common.toc(print=False)
   print(f"Elapsed: {elapsed}s")

**URI Handling**

.. code-block:: python

   from dartpy2.common import Uri

   # Create URI from string
   uri = Uri("file:///path/to/file")
   print(uri.toString())  # "file:///path/to/file"
   print(uri.getPath())   # "/path/to/file"

   # Create URI from filesystem path
   uri2 = Uri.createFromPath("/path/to/file")

Current Status
--------------

dartpy2 is under active development. Currently implemented:

- ✅ Common module (version info, logging, string utils, stopwatch, URI)
- 🚧 Math module (planned)
- 🚧 Dynamics module (planned)
- 🚧 Collision module (planned)
- 🚧 Simulation module (planned)

The implementation is being built incrementally, starting with fundamental modules and progressively adding more functionality.

Transition from dartpy
----------------------

While dartpy2 is being developed, both dartpy and dartpy2 will coexist. Users can import them separately:

.. code-block:: python

   import dartpy   # Current pybind11 bindings
   import dartpy2  # New nanobind bindings

Once dartpy2 is feature-complete, it will become the default Python binding for DART.

Running Tests
-------------

dartpy2 includes comprehensive unit tests. To run them:

.. code-block:: bash

   pixi run test-py2

All tests use pytest and are located in ``python/tests/unit/dartpy2/``.

==========================
dartpy User Guide
==========================

Welcome to the dartpy user guide. This section provides documentation for using
DART's Python bindings.

.. toctree::
   :maxdepth: 1
   :caption: Contents

   installation
   examples
   tutorials
   migration_guide

Getting Started
===============

If you're new to dartpy, we recommend following this order:

1. **Installation** - Set up dartpy on your system
2. **Examples** - Quick start with example code
3. **Tutorials** - Step-by-step learning guides
4. **Migration Guide** - Migrate from dartpy to dartpy2 (DART 6.15 → DART 7)

What is dartpy?
===============

dartpy is the Python binding for DART (Dynamic Animation and Robotics Toolkit).
It provides a Pythonic interface to DART's powerful robotics simulation capabilities.

dartpy2 (DART 7)
================

DART 7 introduces **dartpy2**, a modernized Python binding built with nanobind
that offers:

- **Better Performance**: Faster Python-C++ interop
- **Smaller Binary Size**: More efficient memory usage
- **Modern Python**: Better type hints and IDE support
- **Pythonic API**: Follows PEP 8 conventions with ``snake_case`` methods

If you're using dartpy (DART 6.15), see the :doc:`migration_guide` for upgrading.

Key Features
============

- Load and manipulate articulated rigid body systems
- URDF/SDF file parsing
- Forward and inverse kinematics
- Dynamics simulation with collision detection
- Joint and DOF manipulation
- World simulation with multiple robots

Need Help?
==========

- **GitHub Issues**: https://github.com/dartsim/dart/issues
- **Discussions**: https://github.com/dartsim/dart/discussions
- **Documentation**: https://dartsim.github.io/

Contributing
============

Interested in contributing to dartpy? See our
:doc:`developer guide <../developer_guide/build>` to get started.

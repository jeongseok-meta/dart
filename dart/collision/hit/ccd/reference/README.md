# libccd Reference Implementation

This directory contains the original C implementation of libccd (Continuous Collision Detection library) by Daniel Fiser <danfis@danfis.cz>.

## Purpose

These files serve as a **reference implementation** and are **NOT compiled** as part of the DART build. They are kept for:

1. **Documentation** - Understanding the original algorithm implementations
2. **Reference** - Consulting when modernizing the C++ implementation
3. **Testing** - Comparing behavior with the original implementation

## Files

- **`ccd.c/h`** - Main GJK and EPA algorithms
- **`mpr.c`** - Minkowski Portal Refinement algorithm
- **`support.c/h`** - Support point functions
- **`simplex.h`** - Simplex data structure for GJK
- **`polytope.c/h`** - Polytope for EPA
- **`vec3.c/h`** - 3D vector operations
- **`quat.h`** - Quaternion operations
- **`list.h`** - Linked list utilities
- **`alloc.h`** - Memory allocation utilities
- **`compiler.h`** - Compiler-specific macros
- **`dbg.h`** - Debug utilities
- **`ccd_export.h`** - Export macros

## Modern C++ Implementation

The modern C++ implementation is in the parent directory:

- **`support.hpp`** - Support point structure with Eigen vectors
- **`simplex.hpp`** - Simplex class with STL containers
- **`gjk.hpp`** - GJK algorithm with templates
- **`ccd.hpp`** - Main interface with configuration

## License

The original libccd is distributed under the OSI-approved BSD License.
See the individual files for license details.

## Original Repository

The original libccd can be found at:
- https://github.com/danfis/libccd

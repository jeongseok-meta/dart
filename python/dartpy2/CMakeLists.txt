# Copyright (c) 2011-2025, The DART development contributors
# All rights reserved.
#
# The list of contributors can be found at:
#   https://github.com/dartsim/dart/blob/main/LICENSE
#
# This file is provided under the "BSD-style" License

# Minimal CMakeLists.txt for dartpy2 using nanobind

# Find Python first (required by nanobind)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Try to find nanobind using Python (similar to pybind11 approach)
execute_process(
  COMMAND ${Python3_EXECUTABLE} -m nanobind --cmake_dir
  OUTPUT_VARIABLE nanobind_ROOT
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
  RESULT_VARIABLE nanobind_NOTFOUND
)

if(nanobind_NOTFOUND)
  message(STATUS "nanobind not found. Skipping dartpy2.")
  message(STATUS "To enable dartpy2, install nanobind: pip install nanobind")
  return()
endif()

# Find nanobind using the path from Python
list(APPEND CMAKE_PREFIX_PATH "${nanobind_ROOT}")
find_package(nanobind CONFIG REQUIRED)

message(STATUS "Building dartpy2 with nanobind at ${nanobind_ROOT}")

# Python binding module name
set(nanobind_module dartpy2)

# Collect source files
file(GLOB_RECURSE dartpy2_sources "*.cpp")

# Build a Python extension module using nanobind
nanobind_add_module(${nanobind_module}
  NB_STATIC  # Build a static extension module
  ${dartpy2_sources}
)

# Include directories
target_include_directories(${nanobind_module}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

# Link against DART core library and utils
target_link_libraries(${nanobind_module}
  PRIVATE dart dart-utils-urdf
)

# Link GUI libraries if available
if(TARGET dart-gui)
  target_link_libraries(${nanobind_module} PRIVATE dart-gui)
  message(STATUS "dartpy2: Linking with dart-gui")
else()
  message(WARNING "dartpy2: dart-gui not available - GUI bindings will not work")
endif()

if(TARGET dart-gui-osg)
  target_link_libraries(${nanobind_module} PRIVATE dart-gui-osg)
  message(STATUS "dartpy2: Linking with dart-gui-osg")
else()
  message(WARNING "dartpy2: dart-gui-osg not available - OSG GUI bindings will not work")
endif()

# Set version info
target_compile_definitions(${nanobind_module}
  PRIVATE DARTPY2_VERSION_INFO="${DARTPY_VERSION_INFO}"
)

# Remove debug postfix for dartpy2
set_target_properties(${nanobind_module} PROPERTIES DEBUG_POSTFIX "")

# Set output directory to match expected PYTHONPATH in pixi.toml
set_target_properties(${nanobind_module} PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/python/dartpy2"
)

# Minimal CCD library build test
project(dart_ccd C)

# Collect source files
file(GLOB ccd_sources "*.c")

# Create a static library for testing
add_library(dart-ccd STATIC ${ccd_sources})

# Set include directory
target_include_directories(dart-ccd PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../../..  # So we can use "dart/collision/hit/ccd/..."
)

# Install headers for testing
install(FILES
  ccd.h
  ccd_export.h
  compiler.h
  quat.h
  vec3.h
  DESTINATION include/dart/collision/hit/ccd
)

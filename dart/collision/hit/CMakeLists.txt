# HIT Collision Detection - CCD-based implementation
# Note: Currently includes only libccd for continuous collision detection

# Collect HIT wrapper headers and sources (excluding ccd subdirectory)
file(GLOB hit_headers "*.hpp")
file(GLOB hit_sources "*.cpp")

# NOTE: Original C implementation files (*.c, *.h) are in ccd/reference/
# They serve as reference but are NOT built to avoid conflicts with upstream libccd used by FCL

# Collect CCD library C++ headers (modern template-based implementation)
# All implementations are template-based in headers, no .cpp files needed
set(ccd_headers
  "${CMAKE_CURRENT_LIST_DIR}/ccd/support.hpp"
  "${CMAKE_CURRENT_LIST_DIR}/ccd/simplex.hpp"
  "${CMAKE_CURRENT_LIST_DIR}/ccd/ccd.hpp"
)
set(ccd_sources
  # No source files - all template implementations are in headers
)

# Add wrapper headers and sources to DART
dart_add_core_headers(${hit_headers})
dart_add_core_sources(${hit_sources})

# Add CCD C++ headers and sources to DART
dart_add_core_headers(${ccd_headers})
dart_add_core_sources(${ccd_sources})

# Generate the hit meta header
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "// Automatically generated file by cmake\n\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "// HIT Collision Detector\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "#include \"dart/collision/hit/HitCollisionDetector.hpp\"\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "#include \"dart/collision/hit/HitCollisionGroup.hpp\"\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "#include \"dart/collision/hit/HitCollisionObject.hpp\"\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "#include \"dart/collision/hit/HitTypes.hpp\"\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "\n// HIT convenience header\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "#include \"dart/collision/hit/hit.hpp\"\n")

# Install headers
install(
  FILES ${hit_headers}
        hit.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/all.hpp
  DESTINATION include/dart/collision/hit
  COMPONENT headers
)

# Install CCD headers
install(
  FILES ${ccd_headers}
  DESTINATION include/dart/collision/hit/ccd
  COMPONENT headers
)

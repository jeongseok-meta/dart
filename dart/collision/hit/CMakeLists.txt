# Collect all header and source files
file(GLOB_RECURSE hit_headers "*.h" "*.hpp")
file(GLOB_RECURSE hit_sources "*.cpp")

# Filter out build artifacts, configuration templates, and inline implementation files
list(FILTER hit_headers EXCLUDE REGEX ".*\\.in$")
list(FILTER hit_headers EXCLUDE REGEX ".*-inl\\.h$")
list(FILTER hit_sources EXCLUDE REGEX ".*\\.in$")

# TEMPORARY: Exclude traversal files and related files that depend on missing ShapeBVHDistanceTraversalNode base class
list(FILTER hit_sources EXCLUDE REGEX ".*/traversal/.*\\.cpp$")
list(FILTER hit_headers EXCLUDE REGEX ".*/traversal/.*\\.h$")
list(FILTER hit_sources EXCLUDE REGEX ".*continuous_collision.*\\.cpp$")
list(FILTER hit_headers EXCLUDE REGEX ".*continuous_collision.*\\.h$")
list(FILTER hit_headers EXCLUDE REGEX ".*conservative_advancement_func_matrix.*\\.h$")
list(FILTER hit_headers EXCLUDE REGEX ".*distance_func_matrix.*\\.h$")
list(FILTER hit_sources EXCLUDE REGEX ".*distance\\.cpp$")
list(FILTER hit_headers EXCLUDE REGEX ".*/narrowphase/distance\\.h$")

# Add headers and sources to DART
dart_add_core_headers(${hit_headers})
dart_add_core_sources(${hit_sources})

# Generate configuration header
configure_file(
  config.h.in
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
  @ONLY
)

# Add the generated config.h to headers
dart_add_core_headers("${CMAKE_CURRENT_BINARY_DIR}/config.h")

# Generate the hit meta header manually since dart_generate_component_headers
# doesn't preserve directory structure
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "// Automatically generated file by cmake\n\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/all.hpp" "// HIT Adapter Classes\n")
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
        ${CMAKE_CURRENT_BINARY_DIR}/config.h
  DESTINATION include/dart/collision/hit
  COMPONENT headers
)

# Install license file
install(
  FILES FCL_LICENSE
  DESTINATION include/dart/collision/hit
  COMPONENT headers
)

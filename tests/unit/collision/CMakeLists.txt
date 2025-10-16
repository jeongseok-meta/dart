# Unit tests for collision detection components

dart_add_test("unit" test_ccd test_ccd.cpp)
target_link_libraries(test_ccd dart)

# NOTE: Reference C libccd code in dart/collision/hit/ccd/reference/ is kept
# for documentation purposes only. It should NEVER be modified or built.

# GJK test compares C++ template implementation against upstream libccd
# All dependencies (Eigen, ccd) come transitively from dart target (ccd via fcl)
dart_add_test("unit" test_gjk test_gjk.cpp)
target_link_libraries(test_gjk dart)

# MPR test compares C++ template implementation against upstream libccd
# All dependencies (Eigen, ccd) come transitively from dart target (ccd via fcl)
dart_add_test("unit" test_mpr test_mpr.cpp)
target_link_libraries(test_mpr dart)

# Shape support test validates collision detection with actual DART shapes
# Tests GJK/MPR with Sphere, Box, Capsule shapes
dart_add_test("unit" test_shape_support test_shape_support.cpp)
target_link_libraries(test_shape_support dart)

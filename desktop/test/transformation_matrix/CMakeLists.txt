cmake_minimum_required(VERSION 3.10)

set(TestName TestTransformatinMatrix)

# Create test
add_executable(${TestName}
    test_transformation_matrix.cpp
    test_projection_matrix.cpp
    test_rotation_matrix.cpp
)

# Link to gtest_main to call test cases
target_link_libraries(${TestName} gtest_main)
gtest_discover_tests(TestTransformatinMatrix TestProjectionMatrix TestRotationMatrix)

# Link to the target module
target_link_libraries(${TestName} TransformationMatrix)

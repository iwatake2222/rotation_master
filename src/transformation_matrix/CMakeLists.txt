cmake_minimum_required(VERSION 3.10)

set(LibraryName TransformationMatrix)

# Create library
add_library(${LibraryName}
    transformation_matrix.h transformation_matrix.cpp
    rotation_matrix.h rotation_matrix.cpp
    projection_matrix.h projection_matrix.cpp
)

target_link_libraries(${LibraryName} Matrix)
target_include_directories(${LibraryName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})    # add public so that test module can include header files

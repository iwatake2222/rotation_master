cmake_minimum_required(VERSION 3.10)

set(LibraryName ProjectionMatrix)

# Create library
add_library(${LibraryName}
    projection_matrix.h projection_matrix.cpp
)

target_link_libraries(${LibraryName} Matrix)
target_include_directories(${LibraryName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})    # add public so that test module can include header files

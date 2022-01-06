cmake_minimum_required(VERSION 3.10)

set(LibraryName Matrix)

# Create library
add_library(${LibraryName}
    matrix.h matrix.cpp
)

target_include_directories(${LibraryName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})    # add public so that test module can include header files

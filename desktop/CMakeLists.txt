cmake_minimum_required(VERSION 3.0)

# Create project
set(ProjectName "rotation_master")
project(${ProjectName})

# Compile option
set(CMAKE_INSTALL_PREFIX "./install/" CACHE STRING "install path" FORCE)
include(${CMAKE_SOURCE_DIR}/cmakes/compile_option.cmake)

# Create executable file
add_executable(${ProjectName}
    main.cpp
)

# For OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${ProjectName} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
target_link_libraries(${ProjectName} ${OPENGL_LIBRARIES})
target_include_directories(${ProjectName} PUBLIC ${OPENGL_INCLUDE_DIR})

# For GLFW
set(GLFW_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/glfw/")
add_subdirectory(${GLFW_DIR} glfw)
target_link_libraries(${ProjectName} glfw ${GLFW_LIBRARIES})
target_include_directories(${ProjectName} PUBLIC "${GLFW_DIR}/include/")

# For GLEW
# add_definitions(-DBUILD_UTILS=off)
# add_definitions(-DGLEW_STATIC)
set(GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/glew/")
add_subdirectory("${GLEW_DIR}/build/cmake/" glew)
target_link_libraries(${ProjectName} glew_s)
target_include_directories(${ProjectName} PUBLIC "${GLEW_DIR}/include/")

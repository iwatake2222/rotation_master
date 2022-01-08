cmake_minimum_required(VERSION 3.10)

# Create project
set(ProjectName "rotation_master")
project(${ProjectName})

# Compile option
set(CMAKE_INSTALL_PREFIX "./install/" CACHE STRING "install path" FORCE)
include(${CMAKE_SOURCE_DIR}/cmakes/compile_option.cmake)

# Create executable file
add_executable(${ProjectName}
    main.cpp
    container.h
    ui.h ui.cpp
)

## Add third party libraries
# For OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${ProjectName} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
target_link_libraries(${ProjectName} ${OPENGL_LIBRARIES})
target_include_directories(${ProjectName} PUBLIC ${OPENGL_INCLUDE_DIR})

# For GLFW
set(GLFW_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/glfw/")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${ProjectName} glfw ${GLFW_LIBRARIES})
target_include_directories(${ProjectName} PUBLIC "${GLFW_DIR}/include/")

# For GLEW
# add_definitions(-DBUILD_UTILS=off)
# add_definitions(-DGLEW_STATIC)
set(GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/glew/")
add_subdirectory("${GLEW_DIR}/build/cmake/")
target_link_libraries(${ProjectName} glew_s)
target_include_directories(${ProjectName} PUBLIC "${GLEW_DIR}/include/")

# For ImGui (reference: example_glfw_opengl3/Makefile)
set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/imgui/")
add_library(ImGui
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp 
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_link_libraries(ImGui glfw ${GLFW_LIBRARIES})
target_include_directories(ImGui PUBLIC "${GLFW_DIR}/include/")
target_include_directories(ImGui PUBLIC ${IMGUI_DIR} "${IMGUI_DIR}/backends")
target_link_libraries(${ProjectName} ImGui)

## Add my submodules
add_subdirectory(./matrix)
target_link_libraries(${ProjectName} Matrix)
add_subdirectory(./transformation_matrix)
target_link_libraries(${ProjectName} TransformationMatrix)
add_subdirectory(./gl_helper)
target_link_libraries(${ProjectName} GlHelper)

# Add test module
option(BUILD_TESTS "BUILD_TESTS" ON)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(./test)
endif()

cmake_minimum_required(VERSION 3.10)

set(LibraryName GlHelper)

# Create library
add_library(${LibraryName}
    shader.h shader.cpp
    window.h window.cpp
    shape.h shape.cpp
    object_data.h object_data.cpp
)

target_link_libraries(${LibraryName} Matrix ProjectionMatrix TransformationMatrix)
target_include_directories(${LibraryName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})    # add public so that test module can include header files

target_link_libraries(${LibraryName} glfw ${GLFW_LIBRARIES})
target_include_directories(${LibraryName} PUBLIC "${GLFW_DIR}/include/")
target_link_libraries(${LibraryName} glew_s)
target_include_directories(${LibraryName} PUBLIC "${GLEW_DIR}/include/")

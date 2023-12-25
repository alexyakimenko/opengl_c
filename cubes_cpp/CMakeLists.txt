cmake_minimum_required(VERSION 3.5.0)
project(opengl_c VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(glfw)

include_directories(${GLFW_SOURCE_DIR}/deps)

add_executable(opengl_c 
    src/main.cpp
    ${GLFW_SOURCE_DIR}/deps/glad/gl.h
    )

target_link_libraries(opengl_c
    glfw
    ${OPENGL_LIBRARIES}
)
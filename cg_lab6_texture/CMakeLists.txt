cmake_minimum_required(VERSION 3.3)
project(cg_lab6_texture)

# Find the two frameworks
find_library(opengl_lib OpenGL)
find_library(glut_lib GLUT)

set(frameworks ${opengl_lib} ${glut_lib})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

set(SOURCE_FILES main.cpp)
add_executable(cg_lab6_texture ${SOURCE_FILES})
target_link_libraries(cg_lab6_texture LINK_PUBLIC ${frameworks})
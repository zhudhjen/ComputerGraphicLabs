cmake_minimum_required(VERSION 3.2)
project(cg_lab3_teapot_move)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

# Find the two frameworks
find_library(opengl_lib OpenGL)
find_library(glut_lib GLUT)

# Set variable frameworks to all libraries
set(frameworks ${opengl_lib} ${glut_lib})

set(SOURCE_FILES main.cpp)
add_executable(cg_lab3_teapot_move ${SOURCE_FILES})

# Link ${frameworks} to the project
target_link_libraries(cg_lab3_teapot_move LINK_PUBLIC ${frameworks})

cmake_minimum_required(VERSION 3.2)
project(cg_lab1_hexogram)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

# Find the two frameworks
find_library(opengl_lib OpenGL)
find_library(glut_lib GLUT)

# Set variable frameworks to all libraries
set(frameworks ${opengl_lib} ${glut_lib})

set(SOURCE_FILES main.cpp)
add_executable(cg_lab1_hexogram ${SOURCE_FILES})

# Link ${frameworks} to the project
target_link_libraries(cg_lab1_hexogram LINK_PUBLIC ${frameworks})
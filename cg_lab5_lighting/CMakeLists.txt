cmake_minimum_required(VERSION 3.3)
project(cg_lab5_lighting)

# Find the two frameworks
find_library(opengl_lib OpenGL)
find_library(glut_lib GLUT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated")

set(frameworks ${opengl_lib} ${glut_lib})
set(SOURCE_FILES main.cpp)
add_executable(cg_lab5_lighting ${SOURCE_FILES})
target_link_libraries(cg_lab5_lighting LINK_PUBLIC ${frameworks})
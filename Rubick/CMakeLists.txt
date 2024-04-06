cmake_minimum_required(VERSION 3.17)
project(Rubick)

set(CMAKE_CXX_STANDARD 20)

add_executable(Rubick main.cpp)

target_link_libraries(Rubick -lOpenGL32 -lfreeGLUT)
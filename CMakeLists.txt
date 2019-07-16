cmake_minimum_required(VERSION 3.10)
project(beasm)

set(CMAKE_CXX_STANDARD 17)

add_executable(beasm src/beasm.cpp)					   
#target_compile_options(beasm PRIVATE -Werror -Wall -Wextra)
add_executable(be8 src/be8.cpp)
target_compile_options(be8 PRIVATE -Werror -Wall -Wextra)

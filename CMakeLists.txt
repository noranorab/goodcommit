cmake_minimum_required(VERSION 3.22)

project(goodcommit)

set(CMAKE_CXX_STANDARD 17)

# Add the Clang include directory
include_directories("/usr/lib/llvm-14/include")


add_executable(
    goodcommit main.cpp
)

target_link_libraries(goodcommit -lgit2)
target_link_libraries(goodcommit "/usr/lib/llvm-14/lib/libclang.so")










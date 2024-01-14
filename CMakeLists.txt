cmake_minimum_required(VERSION 3.22)

project(goodcommit)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    goodcommit main.cpp
)

# Link against the library
target_link_libraries(goodcommit -lgit2)


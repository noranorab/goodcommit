cmake_minimum_required(VERSION 3.22)

project(goodcommit)

set(CMAKE_CXX_STANDARD 17)

find_package(Clang REQUIRED)

add_executable(
    goodcommit main.cpp
)

# Link against the library
target_link_libraries(goodcommit -lgit2)
target_include_directories(goodcommit PRIVATE ${CLANG_INCLUDE_DIRS})
target_link_libraries(goodcommit ${CLANG_LIBS})



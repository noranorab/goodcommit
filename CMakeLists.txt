cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(goodcommit)



add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)

# Use llvm-config to get compilation flags
execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Use llvm-config to get linking flags
execute_process(
    COMMAND llvm-config --ldflags
    OUTPUT_VARIABLE LLVM_LD_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND llvm-config --includedir
    OUTPUT_VARIABLE LLVM_CLANG_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND echo ${LLVM_CLANG_INCLUDE_DIR}

)
include_directories("/usr/lib/llvm-14/include")

# Set the compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")

# Set the linking flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LD_FLAGS}")




set(SOURCE_FILE main.cpp)
add_executable(goodcommit ${SOURCE_FILE})

# Include Clang headers
target_include_directories(goodcommit PUBLIC "/usr/lib/llvm-14/include/")

target_link_libraries(goodcommit /home/nora/Téléchargements/llvm-project/build/lib/libclang.so -lgit2)


cmake_minimum_required(VERSION 3.30)
project(file_read_optimisation)

# compilers to use (9.2)

set(CMAKE_CXX_COMPILER  /opt/homebrew/Cellar/gcc/11.2.0_3/bin/g++-11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# openmp flags
#SET(OPENMP_CXX_FLAGS "-O3 -fopenmp --target=x86_64-pc-linux-gnu")
SET(OPENMP_CXX_FLAGS "-O3 -fopenmp")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS}" )


set(CMAKE_CXX_STANDARD 20)

add_executable(file_read_optimisation main.cpp)

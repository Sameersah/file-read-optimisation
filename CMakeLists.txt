cmake_minimum_required(VERSION 3.30)
project(file_read_optimisation)


set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(file_read_optimisation src/main.cpp
        src/CrashRecord.h
        src/ICrashDataProcessor.h
        src/CrashDataProcessor.h
        src/CrashDataProcessor.cpp
)

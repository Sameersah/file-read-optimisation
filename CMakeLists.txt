cmake_minimum_required(VERSION 3.30)
project(file_read_optimisation)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Manually specify OpenMP paths for AppleClang
set(OPENMP_ROOT "/opt/homebrew/opt/libomp")
set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I${OPENMP_ROOT}/include")
set(OpenMP_C_LIB_NAMES "omp")
set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${OPENMP_ROOT}/include")
set(OpenMP_CXX_LIB_NAMES "omp")
set(OpenMP_omp_LIBRARY "${OPENMP_ROOT}/lib/libomp.dylib")

# Set OpenMP flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${OPENMP_ROOT}/lib -lomp")

add_executable(file_read_optimisation
    src/main.cpp
    src/SequentialProcessor/common/CrashRecord.h
    src/ICrashDataProcessor.h
    src/SequentialProcessor/IfStreamProcessor/ProcessorUsingIfStream.h
    src/SequentialProcessor/IfStreamProcessor/ProcessorUsingIfStream.cpp
    src/SequentialProcessor/BufferedFileReadProcessor/ProcessorUsingBufferedFileRead.h
    src/SequentialProcessor/BufferedFileReadProcessor/ProcessorUsingBufferedFileRead.cpp
    src/SequentialProcessor/BufferedFileReadVectorReserveProcessor/ProcessorUsingBufferedFileReadVectorReserve.h
    src/SequentialProcessor/BufferedFileReadVectorReserveProcessor/ProcessorUsingBufferedFileReadVectorReserve.cpp
    src/ParallelProcessor/Experiment1-Threads/ProcessorUsingThreads.h
    src/ParallelProcessor/Experiment1-Threads/ProcessorUsingThreads.cpp
    src/ParallelProcessor/Experiment2-BufferedRead/ParallelBufferRead.h
    src/ParallelProcessor/Experiment2-BufferedRead/ParallelBufferRead.cpp
    src/ParallelProcessor/Experiment3-VectorReserve/ParallelVectorReserve.h
    src/ParallelProcessor/Experiment3-VectorReserve/ParallelVectorReserve.cpp
    src/OptimalProcessor/IfStreamProcessor/OptimalProcessorUsingThreads.h
    src/OptimalProcessor/IfStreamProcessor/OptimalProcessorUsingThreads.cpp
    src/OptimalProcessor/Experiment2-BufferRead/OptimalBufferRead.h
    src/OptimalProcessor/Experiment2-BufferRead/OptimalBufferRead.cpp
    src/OptimalProcessor/Experiment3-VectorReserve/OptimalVectorReserve.h
    src/OptimalProcessor/Experiment3-VectorReserve/OptimalVectorReserve.cpp
)

# Manually link OpenMP
target_include_directories(file_read_optimisation PRIVATE ${OPENMP_ROOT}/include)
target_link_libraries(file_read_optimisation ${OPENMP_ROOT}/lib/libomp.dylib)

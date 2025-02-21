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
        src/common/CrashRecord.h
        src/common/ICrashDataProcessor.h
        src/SequentialProcessor/Experiment1IfStream/ProcessorUsingIfStream.h
        src/SequentialProcessor/Experiment1IfStream/ProcessorUsingIfStream.cpp
        src/SequentialProcessor/Experiment2BufferRead/ProcessorUsingBufferedFileRead.h
        src/SequentialProcessor/Experiment2BufferRead/ProcessorUsingBufferedFileRead.cpp
        src/SequentialProcessor/Experiment3BufferReadVectorReserve/ProcessorUsingBufferedFileReadVectorReserve.h
        src/SequentialProcessor/Experiment3BufferReadVectorReserve/ProcessorUsingBufferedFileReadVectorReserve.cpp
        src/ParallelProcessor/Experiment1MultiThreads/ProcessorUsingThreads.h
        src/ParallelProcessor/Experiment1MultiThreads/ProcessorUsingThreads.cpp
        src/ParallelProcessor/Experiment2BufferedRead/ParallelBufferRead.h
        src/ParallelProcessor/Experiment2BufferedRead/ParallelBufferRead.cpp
        src/ParallelProcessor/Experiment3BufferReadVectorReserve/ParallelVectorReserve.h
        src/ParallelProcessor/Experiment3BufferReadVectorReserve/ParallelVectorReserve.cpp
        src/OptimalProcessor/Experiment1ObjectofArrays/OptimalProcessorUsingThreads.h
        src/OptimalProcessor/Experiment1ObjectofArrays/OptimalProcessorUsingThreads.cpp
        src/OptimalProcessor/Experiment2BufferRead/OptimalBufferRead.h
        src/OptimalProcessor/Experiment2BufferRead/OptimalBufferRead.cpp
        src/OptimalProcessor/Experiment3BufferReadVectorReserve/OptimalVectorReserve.h
        src/OptimalProcessor/Experiment3BufferReadVectorReserve/OptimalVectorReserve.cpp
        src/OptimalProcessor/Experiment4BufferReadVectorReserveThreadLocalBuffer/ProcessorUsingThreadLocalBuffer.h
        src/OptimalProcessor/Experiment4BufferReadVectorReserveThreadLocalBuffer/ProcessorUsingThreadLocalBuffer.cpp
        src/OptimalProcessor/Experiment5BufferReadVectorReserveThreadLocalPartialRead/ProcessorUsingPartialRead.h
        src/OptimalProcessor/Experiment5BufferReadVectorReserveThreadLocalPartialRead/ProcessorUsingPartialRead.cpp
        src/OptimalProcessor/Experiment6BufferReadVectorReserveThreadLocalPartialRead/ProcessorUsingEpochTime.cpp
)

# Manually link OpenMP
target_include_directories(file_read_optimisation PRIVATE ${OPENMP_ROOT}/include)
target_link_libraries(file_read_optimisation ${OPENMP_ROOT}/lib/libomp.dylib)

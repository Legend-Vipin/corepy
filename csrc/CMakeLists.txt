cmake_minimum_required(VERSION 3.15)
project(corepy_kernels VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenBLAS detection
# Using a more robust approach compatible with the user's environment if possible
# But I will follow the requested template first.
find_package(OpenBLAS REQUIRED)

# Include directories
include_directories(${OpenBLAS_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/cpu/elementwise.cpp
    src/cpu/dummy.cpp
    src/cpu/matmul.cpp
    src/cpu/reduce.cpp
    src/cpu/blas_kernels.cpp
)

# Create static library
add_library(corepy_kernels STATIC ${SOURCES})
target_compile_definitions(corepy_kernels PRIVATE COREPY_USE_OPENBLAS)
target_link_libraries(corepy_kernels ${OpenBLAS_LIBRARIES})
target_include_directories(corepy_kernels PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable AVX2 SIMD optimizations if supported
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    if(MSVC)
        target_compile_options(corepy_kernels PRIVATE /arch:AVX2)
    else()
        target_compile_options(corepy_kernels PRIVATE -mavx2 -mfma)
    endif()
    target_compile_definitions(corepy_kernels PRIVATE COREPY_USE_AVX2)
endif()

# Installation (optional)
install(TARGETS corepy_kernels DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)


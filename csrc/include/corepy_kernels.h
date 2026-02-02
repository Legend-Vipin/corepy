#pragma once

// ============================================================================
// Corepy C++ Kernels: Pure Execution Layer
// ============================================================================
//
// This is the C++ layer of Corepy's 3-layer architecture:
//
//   Python (UX) → Rust (Brain) → C++ (Muscle)
//
// RESPONSIBILITIES (see docs/execution_model.md):
// - SIMD kernels (AVX2/AVX-512/NEON)
// - GPU kernels (CUDA/Metal - future)
// - Pure pointer arithmetic
// - Zero safety checks (trusts Rust)
// - Zero Python awareness
//
// RULES:
// - All functions must be extern "C" for Rust FFI
// - No safety checks (Rust validates before calling)
// - No Python dependencies (no Python.h)
// - No memory management (Rust owns lifetimes)
// - No execution order decisions (Rust schedules)

#include <cstddef>
#include <cstdint>

extern "C" {
    // ========================================================================
    // Demo Kernels (Backward Compatibility)
    // ========================================================================
    
    int add_one_kernel(int x);
    
    // ========================================================================
    // Reduction Operations (src/cpu/reduce.cpp)
    // ========================================================================
    
    /// Returns true if all elements in data are non-zero (truthy)
    /// 
    /// SIMD Implementation: Uses AVX2 to check 32 bytes at once
    /// Fallback: Scalar loop for non-AVX2 systems
    /// 
    /// Arguments:
    ///   data: Pointer to u8 buffer (0=false, 1=true)
    ///   count: Number of elements
    /// 
    /// Returns: true if all non-zero, false otherwise
    /// 
    /// Safety Contract with Rust:
    ///   - data is valid for `count` bytes
    ///   - data is aligned (no requirement, uses unaligned loads)
    ///   - data lifetime exceeds this function call
    ///   - No concurrent mutations
    bool all_bool_cpu(const uint8_t* data, size_t count);
    
    /// Returns true if any element in data is non-zero (truthy)
    /// Early-exit optimization: returns immediately on first true value
    bool any_bool_cpu(const uint8_t* data, size_t count);
    
    /// Sum reduction on f32 array
    /// Uses Kahan summation algorithm for improved precision
    float sum_f32_cpu(const float* data, size_t count);
    
    /// Sum reduction on i32 array
    int32_t sum_i32_cpu(const int32_t* data, size_t count);
    
    /// Mean (average) of f32 array
    /// Returns sum / count
    float mean_f32_cpu(const float* data, size_t count);

    
    // ========================================================================
    // Element-wise Operations (src/cpu/elementwise.cpp)
    // ========================================================================
    void add_f32_cpu(const float* a, const float* b, float* out, size_t count);
    void sub_f32_cpu(const float* a, const float* b, float* out, size_t count);
    void mul_f32_cpu(const float* a, const float* b, float* out, size_t count);
    void div_f32_cpu(const float* a, const float* b, float* out, size_t count);

    
    // ========================================================================
    // Matrix Operations (src/cpu/matmul.cpp)
    // ========================================================================
    float dot_product_f32_cpu(const float* a, const float* b, size_t count);
    void matmul_f32_cpu(const float* a, const float* b, float* c,
                        size_t m, size_t k, size_t n);

    // ========================================================================
    // Backend Control
    // ========================================================================
    void corepy_set_num_threads(int num_threads);
    bool corepy_is_blas_enabled();
}

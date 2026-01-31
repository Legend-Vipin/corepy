#include "corepy_kernels.h"
#include <cstdint>
#include <cstddef>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
  #ifdef __AVX2__
    #include <immintrin.h>
  #endif
#endif

// ============================================================================
// Element-wise Operations: SIMD-optimized kernels
// ============================================================================
// These operations perform element-wise arithmetic on float arrays.
// AVX2 implementation processes 8 floats per iteration for ~8x speedup.

// ============================================================================
// add_f32_cpu: out[i] = a[i] + b[i]
// ============================================================================

extern "C" void add_f32_cpu(const float* a, const float* b, float* out, size_t count) {
#ifdef __AVX2__
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    // Process 8 floats per iteration
    for (size_t i = 0; i < avx_count; ++i) {
        __m256 va = _mm256_loadu_ps(a + i * 8);
        __m256 vb = _mm256_loadu_ps(b + i * 8);
        __m256 vout = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(out + i * 8, vout);
    }
    
    // Process remainder
    for (size_t i = avx_count * 8; i < count; ++i) {
        out[i] = a[i] + b[i];
    }
    
#else
    // Scalar fallback
    for (size_t i = 0; i < count; ++i) {
        out[i] = a[i] + b[i];
    }
#endif
}

// ============================================================================
// sub_f32_cpu: out[i] = a[i] - b[i]
// ============================================================================

extern "C" void sub_f32_cpu(const float* a, const float* b, float* out, size_t count) {
#ifdef __AVX2__
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256 va = _mm256_loadu_ps(a + i * 8);
        __m256 vb = _mm256_loadu_ps(b + i * 8);
        __m256 vout = _mm256_sub_ps(va, vb);
        _mm256_storeu_ps(out + i * 8, vout);
    }
    
    for (size_t i = avx_count * 8; i < count; ++i) {
        out[i] = a[i] - b[i];
    }
    
#else
    for (size_t i = 0; i < count; ++i) {
        out[i] = a[i] - b[i];
    }
#endif
}

// ============================================================================
// mul_f32_cpu: out[i] = a[i] * b[i]
// ============================================================================

extern "C" void mul_f32_cpu(const float* a, const float* b, float* out, size_t count) {
#ifdef __AVX2__
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256 va = _mm256_loadu_ps(a + i * 8);
        __m256 vb = _mm256_loadu_ps(b + i * 8);
        __m256 vout = _mm256_mul_ps(va, vb);
        _mm256_storeu_ps(out + i * 8, vout);
    }
    
    for (size_t i = avx_count * 8; i < count; ++i) {
        out[i] = a[i] * b[i];
    }
    
#else
    for (size_t i = 0; i < count; ++i) {
        out[i] = a[i] * b[i];
    }
#endif
}

// ============================================================================
// div_f32_cpu: out[i] = a[i] / b[i]
// ============================================================================

extern "C" void div_f32_cpu(const float* a, const float* b, float* out, size_t count) {
#ifdef __AVX2__
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256 va = _mm256_loadu_ps(a + i * 8);
        __m256 vb = _mm256_loadu_ps(b + i * 8);
        __m256 vout = _mm256_div_ps(va, vb);
        _mm256_storeu_ps(out + i * 8, vout);
    }
    
    for (size_t i = avx_count * 8; i < count; ++i) {
        out[i] = a[i] / b[i];
    }
    
#else
    for (size_t i = 0; i < count; ++i) {
        out[i] = a[i] / b[i];
    }
#endif
}

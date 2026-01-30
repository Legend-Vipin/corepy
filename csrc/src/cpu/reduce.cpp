#include "corepy_kernels.h"
#include <cstdint>
#include <cstddef>

#ifdef __AVX2__
#include <immintrin.h>
#endif

extern "C" bool all_bool_cpu(const uint8_t* data, size_t count) {
    // RUST LAYER CONTRACT:
    // - data is valid, non-null pointer
    // - count is correct element count
    // - No validation needed (Rust already did it)
    
    // C++ LAYER RESPONSIBILITY:
    // - Pure execution with SIMD optimization
    // - Trust inputs are valid
    
#ifdef __AVX2__
    // AVX2 path: Check 32 bytes at once
    // Strategy: Use vectorized comparison to find any zeros (false values)
    size_t avx_count = count / 32;
    size_t remainder = count % 32;
    
    // Process 32 bytes per iteration using AVX2
    for (size_t i = 0; i < avx_count; ++i) {
        // Load 32 bytes (256 bits) of boolean data
        __m256i chunk = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(data + i * 32)
        );
        
        // Compare with zero to find any false values
        __m256i zeros = _mm256_setzero_si256();
        __m256i cmp = _mm256_cmpeq_epi8(chunk, zeros);
        
        // Extract mask: if any byte is zero, corresponding bit is set
        int mask = _mm256_movemask_epi8(cmp);
        
        // If any comparison matched (any zero found), return false immediately
        if (mask != 0) {
            return false;
        }
    }
    
    // Process remaining elements with scalar loop
    for (size_t i = avx_count * 32; i < count; ++i) {
        if (data[i] == 0) {
            return false;
        }
    }
    
    // All elements are non-zero (truthy)
    return true;
    
#else
    // Scalar fallback for systems without AVX2
    // Simple loop checking each element
    for (size_t i = 0; i < count; ++i) {
        if (data[i] == 0) {
            return false;
        }
    }
    return true;
#endif
}

// ============================================================================
// any_bool_cpu: Returns true if ANY element is non-zero
// ============================================================================

extern "C" bool any_bool_cpu(const uint8_t* data, size_t count) {
    // Early-exit optimization: return true on first non-zero element
    
#ifdef __AVX2__
    // AVX2 path: Check 32 bytes at once
    size_t avx_count = count / 32;
    size_t remainder = count % 32;
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256i chunk = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(data + i * 32)
        );
        
        // Compare with zero
        __m256i zeros = _mm256_setzero_si256();
        __m256i cmp = _mm256_cmpeq_epi8(chunk, zeros);
        
        // If not all bytes are zero, at least one is non-zero
        int mask = _mm256_movemask_epi8(cmp);
        
        // mask == -1 means all bytes were zero, otherwise we found a non-zero
        if (mask != -1) {
            return true;
        }
    }
    
    // Check remainder
    for (size_t i = avx_count * 32; i < count; ++i) {
        if (data[i] != 0) {
            return true;
        }
    }
    
    return false;
    
#else
    // Scalar fallback
    for (size_t i = 0; i < count; ++i) {
        if (data[i] != 0) {
            return true;
        }
    }
    return false;
#endif
}

// ============================================================================
// sum_f32_cpu: Sum reduction with Kahan summation for precision
// ============================================================================

extern "C" float sum_f32_cpu(const float* data, size_t count) {
    if (count == 0) {
        return 0.0f;
    }
    
#ifdef __AVX2__
    // AVX2 path: Process 8 floats at once
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    // Accumulator for AVX2 sums
    __m256 sum_vec = _mm256_setzero_ps();
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256 chunk = _mm256_loadu_ps(data + i * 8);
        sum_vec = _mm256_add_ps(sum_vec, chunk);
    }
    
    // Horizontal sum: reduce 8 lanes to single value
    __m128 low = _mm256_castps256_ps128(sum_vec);
    __m128 high = _mm256_extractf128_ps(sum_vec, 1);
    __m128 sum128 = _mm_add_ps(low, high);
    
    sum128 = _mm_hadd_ps(sum128, sum128);
    sum128 = _mm_hadd_ps(sum128, sum128);
    
    float sum = _mm_cvtss_f32(sum128);
    
    // Add remainder with Kahan summation
    float c = 0.0f;
    for (size_t i = avx_count * 8; i < count; ++i) {
        float y = data[i] - c;
        float t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    
    return sum;
    
#else
    // Scalar path with Kahan summation
    float sum = 0.0f;
    float c = 0.0f;
    
    for (size_t i = 0; i < count; ++i) {
        float y = data[i] - c;
        float t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    
    return sum;
#endif
}

// ============================================================================
// sum_i32_cpu: Integer sum reduction
// ============================================================================

extern "C" int32_t sum_i32_cpu(const int32_t* data, size_t count) {
    if (count == 0) {
        return 0;
    }
    
#ifdef __AVX2__
    // AVX2 path: Process 8 int32s at once
    size_t avx_count = count / 8;
    size_t remainder = count % 8;
    
    __m256i sum_vec = _mm256_setzero_si256();
    
    for (size_t i = 0; i < avx_count; ++i) {
        __m256i chunk = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(data + i * 8)
        );
        sum_vec = _mm256_add_epi32(sum_vec, chunk);
    }
    
    // Horizontal sum
    __m128i low = _mm256_castsi256_si128(sum_vec);
    __m128i high = _mm256_extracti128_si256(sum_vec, 1);
    __m128i sum128 = _mm_add_epi32(low, high);
    
    sum128 = _mm_hadd_epi32(sum128, sum128);
    sum128 = _mm_hadd_epi32(sum128, sum128);
    
    int32_t sum = _mm_extract_epi32(sum128, 0);
    
    // Add remainder
    for (size_t i = avx_count * 8; i < count; ++i) {
        sum += data[i];
    }
    
    return sum;
    
#else
    // Scalar path
    int32_t sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += data[i];
    }
    return sum;
#endif
}

// ============================================================================
// mean_f32_cpu: Mean (average) of f32 array
// ============================================================================

extern "C" float mean_f32_cpu(const float* data, size_t count) {
    if (count == 0) {
        return 0.0f;
    }
    
    float sum = sum_f32_cpu(data, count);
    return sum / static_cast<float>(count);
}


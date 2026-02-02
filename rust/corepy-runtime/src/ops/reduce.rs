// ============================================================================
// Operations: Reduction Kernels
// ============================================================================
// This module handles reduction operations (all, any, sum, mean, etc.)
//
// RESPONSIBILITIES:
// - Validate operation parameters
// - Dispatch to appropriate C++ kernel
// - Handle different data types and backends

/// Threshold for parallel dispatch (elements)
/// Below this: sequential C++ kernel  
/// Above this: Rayon parallel reduction
const PARALLEL_THRESHOLD_F32: usize = 1_000_000;
const PARALLEL_THRESHOLD_I32: usize = 1_000_000;

// FFI declaration for C++ kernels
extern "C" {
    /// CPU kernel for all() reduction
    /// Returns true if all elements in data are non-zero
    /// 
    /// C++ signature: bool all_bool_cpu(const uint8_t* data, size_t count)
    /// Location: csrc/kernels/reduction.cpp
    pub fn all_bool_cpu(data_ptr: *const u8, count: usize) -> bool;
    
    /// CPU kernel for any() reduction
    /// Returns true if any element in data is non-zero (truthy)
    pub fn any_bool_cpu(data_ptr: *const u8, count: usize) -> bool;
    
    /// CPU kernel for sum() reduction on f32
    /// Returns sum of all elements
    pub fn sum_f32_cpu(data_ptr: *const f32, count: usize) -> f32;
    
    /// CPU kernel for sum() reduction on i32
    pub fn sum_i32_cpu(data_ptr: *const i32, count: usize) -> i32;
    
    /// CPU kernel for mean() reduction on f32
    /// Returns arithmetic mean of all elements
    pub fn mean_f32_cpu(data_ptr: *const f32, count: usize) -> f32;
}

/// Dispatch all() operation to CPU kernel
/// 
/// This is a thin wrapper around the C++ FFI that ensures safety.
/// 
/// # Safety
/// Caller must ensure:
/// - data_ptr is valid for `count` bytes
/// - data_ptr lifetime exceeds this function call
/// - No concurrent mutations to the buffer
pub unsafe fn all_bool_cpu_dispatch(data_ptr: *const u8, count: usize) -> bool {
    use crate::scheduler::arena::with_arena;
    
    // RUST LAYER RESPONSIBILITY:
    // We validated the pointer and count in ffi/python.rs
    // Now we trust the C++ layer to execute correctly
    //
    // PERFORMANCE: Arena scope ensures thread-local allocations are available
    // for future optimizations (e.g., temporary buffers)
    
    with_arena(|_arena| {
        all_bool_cpu(data_ptr, count)
    })
}

/// Dispatch any() operation to CPU kernel
pub unsafe fn any_bool_cpu_dispatch(data_ptr: *const u8, count: usize) -> bool {
    use crate::scheduler::arena::with_arena;
    
    with_arena(|_arena| {
        any_bool_cpu(data_ptr, count)
    })
}

/// Dispatch sum() operation to CPU kernel (f32)
/// Automatically parallelizes for large arrays (>100K elements)
pub unsafe fn sum_f32_cpu_dispatch(data_ptr: *const f32, count: usize) -> f32 {
    use crate::scheduler::arena::with_arena;
    
    with_arena(|_arena| {
        if count >= PARALLEL_THRESHOLD_F32 {
            // Parallel path: use Rayon
            parallel_sum_f32_cpu(data_ptr, count)
        } else {
            // Sequential path: direct C++ kernel
            sum_f32_cpu(data_ptr, count)
        }
    })
}

/// Parallel sum implementation using Rayon
unsafe fn parallel_sum_f32_cpu(data_ptr: *const f32, count: usize) -> f32 {
    use rayon::prelude::*;
    
    let slice = std::slice::from_raw_parts(data_ptr, count);
    
    // Divide work across CPUs
    let num_threads = num_cpus::get();
    let chunk_size = (count + num_threads - 1) / num_threads;
    
    // Parallel reduction
    slice.par_chunks(chunk_size)
         .map(|chunk| unsafe {
             // Call C++ AVX2 kernel per chunk instead of scalar Rust sum
             sum_f32_cpu(chunk.as_ptr(), chunk.len())
         })
         .sum()
}

/// Dispatch sum() operation to CPU kernel (i32)
/// Automatically parallelizes for large arrays (>100K elements)
pub unsafe fn sum_i32_cpu_dispatch(data_ptr: *const i32, count: usize) -> i32 {
    use crate::scheduler::arena::with_arena;
    
    with_arena(|_arena| {
        if count >= PARALLEL_THRESHOLD_I32 {
            parallel_sum_i32_cpu(data_ptr, count)
        } else {
            sum_i32_cpu(data_ptr, count)
        }
    })
}

/// Parallel sum implementation for i32
unsafe fn parallel_sum_i32_cpu(data_ptr: *const i32, count: usize) -> i32 {
    use rayon::prelude::*;
    
    let slice = std::slice::from_raw_parts(data_ptr, count);
    let num_threads = num_cpus::get();
    let chunk_size = (count + num_threads - 1) / num_threads;
    
    slice.par_chunks(chunk_size)
         .map(|chunk| unsafe {
             // Call C++ SIMD kernel per chunk
             sum_i32_cpu(chunk.as_ptr(), chunk.len())
         })
         .sum()
}

/// Dispatch mean() operation to CPU kernel (f32)
/// Automatically parallelizes for large arrays (>100K elements)
pub unsafe fn mean_f32_cpu_dispatch(data_ptr: *const f32, count: usize) -> f32 {
    use crate::scheduler::arena::with_arena;
    
    with_arena(|_arena| {
        if count >= PARALLEL_THRESHOLD_F32 {
            // Parallel sum + divide
            let sum = parallel_sum_f32_cpu(data_ptr, count);
            sum / (count as f32)
        } else {
            mean_f32_cpu(data_ptr, count)
        }
    })
}

// ============================================================================
// Operations: Matrix Multiplication
// ============================================================================

// FFI declaration for C++ kernel
extern "C" {
    /// AVX2-optimized dot product kernel
    pub fn dot_product_f32_cpu(a: *const f32, b: *const f32, count: usize) -> f32;

    /// Matrix Multiplication (Native or BLAS)
    pub fn matmul_f32_cpu(a: *const f32, b: *const f32, c: *mut f32, m: usize, k: usize, n: usize);

    /// Check if BLAS backend is active
    pub fn corepy_is_blas_enabled() -> bool;

    /// Set number of threads for the backend
    #[allow(dead_code)]
    pub fn corepy_set_num_threads(num_threads: i32);
}

/// Safety wrapper for pointers to be Send/Sync for Rayon
struct SendPtr<T>(*const T);
unsafe impl<T> Send for SendPtr<T> {}
unsafe impl<T> Sync for SendPtr<T> {}
impl<T> SendPtr<T> {
    #[inline]
    fn ptr(&self) -> *const T { self.0 }
}

struct SendPtrMut<T>(*mut T);
unsafe impl<T> Send for SendPtrMut<T> {}
unsafe impl<T> Sync for SendPtrMut<T> {}
impl<T> SendPtrMut<T> {
    #[inline]
    fn ptr(&self) -> *mut T { self.0 }
}

/// Dispatch dot product operation to CPU kernel
pub unsafe fn dot_product_f32_cpu_dispatch(a: *const f32, b: *const f32, count: usize) -> f32 {
    use crate::scheduler::arena::with_arena;
    with_arena(|_arena| {
        dot_product_f32_cpu(a, b, count)
    })
}

/// Dispatch 2D matrix multiplication to CPU kernel
pub unsafe fn matmul_f32_cpu_dispatch(
    a: *const f32, b: *const f32, c: *mut f32,
    m: usize, k: usize, n: usize
) {
    use crate::scheduler::arena::with_arena;
    use crate::backend::{get_policy, BackendPolicy, record_dispatch, record_detailed_dispatch};
    
    let policy = get_policy();
    
    let use_blas = match policy {
        BackendPolicy::BLAS => true,     // User forced BLAS
        BackendPolicy::OPENBLAS => true, // User forced OpenBLAS
        BackendPolicy::DEFAULT => {
            // Heuristic flip point based on benchmarks (Large matrices > 256x256)
            m > 256 || n > 256 || k > 256
        }
        _ => false, // CUDA etc not handled here yet
    };

    // Check if we should use BLAS or native Rayon dispatch
    if use_blas && corepy_is_blas_enabled() {
        record_dispatch(1); // OpenBLAS ID (Mapping: 1=OpenBLAS)
        record_detailed_dispatch(1, "matmul", m, n, k, policy);
        
        // Direct BLAS call - OpenBLAS handles its own threading efficiently
        with_arena(|_arena| {
            matmul_f32_cpu(a, b, c, m, k, n);
        });
    } else {
        record_dispatch(0); // Corepy ID
        record_detailed_dispatch(0, "matmul", m, n, k, policy);
        
        // Fallback to naive Rayon parallel dispatch for custom AVX2/Scalar kernels
        // Fallback to naive Rayon parallel dispatch for custom AVX2/Scalar kernels
        use rayon::prelude::*;
        
        let a_wrap = SendPtr(a);
        let b_wrap = SendPtr(b);
        let c_wrap = SendPtrMut(c);

        with_arena(|_arena| {
            let num_threads = num_cpus::get();
            let rows_per_thread = (m + num_threads - 1) / num_threads;

            (0..m).into_par_iter()
                  .chunks(rows_per_thread)
                  .for_each(move |row_indices| {
                      let start_row = row_indices[0];
                      let num_rows = row_indices.len();
                      
                      unsafe {
                          matmul_f32_cpu(
                              a_wrap.ptr().add(start_row * k),
                              b_wrap.ptr(),
                              c_wrap.ptr().add(start_row * n),
                              num_rows, k, n
                          );
                      }
                  });
        });
    }
}

#include "corepy_kernels.h"

#ifdef COREPY_USE_OPENBLAS
#include <cblas.h>

extern "C" void openblas_set_num_threads(int num_threads);

namespace corepy::backend::openblas {

float dot_product_f32(const float* a, const float* b, size_t count) {
    return cblas_sdot(static_cast<int>(count), a, 1, b, 1);
}

void matmul_f32(const float* a, const float* b, float* c,
                size_t m, size_t k, size_t n) {
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                static_cast<int>(m), static_cast<int>(n), static_cast<int>(k),
                1.0f, a, static_cast<int>(k), b, static_cast<int>(n), 0.0f, c, static_cast<int>(n));
}

void set_num_threads(int num_threads) {
    #include <openblas_config.h>
    #if defined(OPENBLAS_VERSION)
    openblas_set_num_threads(num_threads);
    #endif
}

} // namespace corepy::backend::openblas

extern "C" {

float dot_product_f32_cpu(const float* a, const float* b, size_t count) {
    return corepy::backend::openblas::dot_product_f32(a, b, count);
}

void matmul_f32_cpu(const float* a, const float* b, float* c,
                    size_t m, size_t k, size_t n) {
    corepy::backend::openblas::matmul_f32(a, b, c, m, k, n);
}

void corepy_set_num_threads(int num_threads) {
    corepy::backend::openblas::set_num_threads(num_threads);
}

bool corepy_is_blas_enabled() {
    return true;
}

} // extern "C"

#endif // COREPY_USE_OPENBLAS

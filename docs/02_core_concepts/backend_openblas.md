# OpenBLAS Backend for Corepy

Corepy uses OpenBLAS as a high-performance, portable linear algebra backend. This replaces or augments custom SIMD (AVX2) kernels to ensure compatibility across x86, ARM, and systems without advanced vector extensions.

## Architecture

The backend is abstracted into C++ namespaces and exposed via a unified Rust FFI:

- `corepy::backend::openblas`: Uses standard CBLAS calls (`cblas_sgemm`, `cblas_sdot`).
- `corepy::backend::avx2`: Optimized native kernels for x86_64 systems.

At runtime, the Rust layer queries the active backend using `corepy_is_blas_enabled()` and dispatches accordingly. If OpenBLAS is active, it handles its own internal multi-threading. If the native backend is active, Corepy uses Rayon for parallel dispatch.

## Build Instructions

### Linux (Debian/Ubuntu)
Install the system OpenBLAS development files:
```bash
sudo apt-get update && sudo apt-get install -y libopenblas-dev
```

### Build Flags
Use the following CMake flags to control the backend:

- `COREPY_USE_OPENBLAS=ON`: Enable OpenBLAS (default).
- `COREPY_DISABLE_AVX2=ON`: Force-disable native AVX2 kernels.

Example build command:
```bash
cmake -B build -DCOREPY_USE_OPENBLAS=ON
cmake --build build
```

## Performance Notes

OpenBLAS provides highly optimized, handwritten assembly kernels for many architectures. In our benchmarks, the OpenBLAS backend achieves:

- **116 GFLOPS** on standard x86_64 hardware (1024x1024 float32 matmul).
- **Parity with NumPy**, ensuring Corepy doesn't introduce linear algebra bottlenecks.
- **Improved Portability**: Automatic optimization for ARM (Apple Silicon, Graviton) and older x86 CPUs.

## Thread Control

OpenBLAS threading can be controlled via the environment variable:
```bash
export OPENBLAS_NUM_THREADS=4
```
Or programmatically through the Corepy API (exposed via Rust FFI):
```rust
corepy_set_num_threads(4);
```

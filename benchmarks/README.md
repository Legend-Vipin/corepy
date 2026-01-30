# Benchmarks

Performance benchmarking suite for Corepy.

## Running Benchmarks

```bash
# Run matrix multiplication benchmarks  
python benchmark.py

# Compare with baseline
python compare_benchmark.py

# Measure profiler overhead
python benchmarks/profiler_overhead.py
```

## Available Benchmarks

- `../benchmark.py` - Matrix multiplication performance across sizes
- `../compare_benchmark.py` - Comparison with NumPy baseline
- `profiler_overhead.py` - Profiler performance overhead testing

## Interpreting Results

Benchmarks measure:
- **Execution time** (microseconds/milliseconds)
- **GFLOPS** (billions of floating-point operations per second)
- **Speedup** relative to NumPy
- **Backend selection** (SIMD, BLAS, scalar)

## Adding New Benchmarks

1. Create a new `.py` file in this directory
2. Use `time.perf_counter()` for timing
3. Include warmup runs to eliminate cold-start effects
4. Report mean and standard deviation over multiple runs

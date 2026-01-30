#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== CorePy Performance Build ==="

echo "Phase 1: Cleaning..."
rm -rf csrc/build
rm -rf rust/target

echo ""
echo "Phase 2: Building C++ Kernels..."
mkdir -p csrc/build
cd csrc/build
cmake .. -DCMAKE_BUILD_TYPE=Release -Wno-dev
make -j$(nproc)
cd ../..

echo ""
echo "Phase 3: Building Rust Runtime..."
cd rust/corepy-runtime
maturin develop --release
cd ../..

echo ""
echo "Phase 4: Performance Verification..."
if command -v uv >/dev/null 2>&1; then
    uv run python -c "
import corepy
import numpy as np
import time

print('=== Performance Test ===')
print(f'Backend: {corepy.get_backend_policy()}')

# Test different sizes
sizes = [(100, 100), (512, 512), (1024, 1024)]
for m, n in sizes:
    a = corepy.Tensor(np.random.randn(m, n).astype(np.float32))
    b = corepy.Tensor(np.random.randn(n, m).astype(np.float32))
    
    start = time.time()
    c = a.matmul(b)
    elapsed = time.time() - start
    
    print(f'{m}x{n}: {elapsed*1000:.2f}ms - {corepy.explain_last_dispatch()}')

print('=== Performance test complete ===')
"
else
    python3 -c "
import corepy
import numpy as np
import time

print('=== Performance Test ===')
print(f'Backend: {corepy.get_backend_policy()}')

# Test different sizes
sizes = [(100, 100), (512, 512), (1024, 1024)]
for m, n in sizes:
    a = corepy.Tensor(np.random.randn(m, n).astype(np.float32))
    b = corepy.Tensor(np.random.randn(n, m).astype(np.float32))
    
    start = time.time()
    c = a.matmul(b)
    elapsed = time.time() - start
    
    print(f'{m}x{n}: {elapsed*1000:.2f}ms - {corepy.explain_last_dispatch()}')

print('=== Performance test complete ===')
"
fi

echo ""
echo "Phase 5: Cleanup..."
find csrc/build -name "*.o" -type f -delete 2>/dev/null || true

echo ""
echo "=== Build Complete ==="

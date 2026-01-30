#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== CorePy Clean Rebuild ==="

echo "Cleaning previous builds..."
rm -rf csrc/build
rm -rf rust/target

echo ""
echo "Building C++ kernels..."
mkdir -p csrc/build
cd csrc/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
cd ../..

echo ""
echo "Building Rust runtime..."
cd rust/corepy-runtime
maturin develop --release
cd ../..

echo ""
echo "=== Verification ==="
if command -v uv >/dev/null 2>&1; then
    uv run python -c "
import corepy
import numpy as np

print(f'Backend: {corepy.get_backend_policy()}')

# Test matmul
a = corepy.Tensor(np.random.randn(100, 100).astype(np.float32))
b = corepy.Tensor(np.random.randn(100, 100).astype(np.float32))
c = a.matmul(b)
print(f'Matmul test: {corepy.explain_last_dispatch()}')
"
else
    python3 -c "
import corepy
import numpy as np

print(f'Backend: {corepy.get_backend_policy()}')

# Test matmul
a = corepy.Tensor(np.random.randn(100, 100).astype(np.float32))
b = corepy.Tensor(np.random.randn(100, 100).astype(np.float32))
c = a.matmul(b)
print(f'Matmul test: {corepy.explain_last_dispatch()}')
"
fi

echo ""
echo "=== Rebuild Complete! ==="

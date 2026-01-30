#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=== CorePy Build Script ==="

# Check tools
command -v cmake >/dev/null 2>&1 || { echo "❌ cmake required"; exit 1; }
command -v maturin >/dev/null 2>&1 || { echo "❌ maturin required"; exit 1; }

# Step 1: C++ kernels
echo ""
echo "Step 1/2: Building C++ kernels..."
mkdir -p csrc/build
cd csrc/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
echo "✅ C++ kernels built"
cd ../..

# Step 2: Rust runtime (installs everything via maturin)
echo ""
echo "Step 2/2: Building Rust runtime..."
cd rust/corepy-runtime
maturin develop --release
echo "✅ Rust runtime installed"
cd ../..

# Verify with UV-first approach
echo ""
echo "=== Verification ==="
if command -v uv >/dev/null 2>&1; then
    uv run python -c "
import corepy as cp
print(f'✅ Corepy loaded')
print(f'✅ Backend: {cp.get_backend_policy()}')
print(f'✅ Test: {cp.Tensor([1.0, 2.0, 3.0])}')
"
else
    python3 -c "
import corepy as cp
print(f'✅ Corepy loaded')
print(f'✅ Backend: {cp.get_backend_policy()}')
print(f'✅ Test: {cp.Tensor([1.0, 2.0, 3.0])}')
"
fi

echo ""
echo "=== Build Complete! ==="

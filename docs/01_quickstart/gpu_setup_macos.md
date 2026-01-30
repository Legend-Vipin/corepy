# 🍎 GPU Setup Guide - macOS (Apple Silicon)

This guide will help you set up **Corepy** on macOS with **Apple Silicon** (M1, M2, M3, M4 chips) to leverage GPU acceleration through **Metal**.

> [!NOTE]
> Apple Silicon Macs use the **Metal** framework for GPU acceleration, not CUDA. The M-series chips have unified memory, making GPU operations extremely efficient.

---

## 📋 Prerequisites

Before you begin, ensure you have:

1. **Mac with Apple Silicon**
   - M1, M1 Pro, M1 Max, M1 Ultra
   - M2, M2 Pro, M2 Max, M2 Ultra
   - M3, M3 Pro, M3 Max
   - M4, M4 Pro, M4 Max
2. **macOS 12.0 (Monterey) or newer** (macOS 13+ recommended)
3. **Python 3.10 or newer**
4. **Xcode Command Line Tools**
5. **Homebrew** (recommended)

---

## 📊 Setup Process Overview

**Apple Silicon Setup is Simple:**
1. Install Xcode Command Line Tools
2. Install Homebrew (if not already installed)
3. Install build dependencies (CMake, Rust)
4. Clone and build Corepy with Metal support
5. Verify GPU detection

**No separate GPU drivers or CUDA needed!** ✨

---

## 🚀 Step-by-Step Installation

### Step 1: Install Xcode Command Line Tools

Xcode Command Line Tools provides the C++ compiler and build tools:

```bash
# Install Xcode Command Line Tools
xcode-select --install
```

A dialog will appear. Click "Install" and accept the license agreement.

**Verify installation:**
```bash
xcode-select -p
```

Expected output: `/Library/Developer/CommandLineTools` or `/Applications/Xcode.app/Contents/Developer`

---

### Step 2: Install Homebrew (if not installed)

Homebrew is the package manager for macOS:

```bash
# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Add Homebrew to PATH (for Apple Silicon)
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"
```

**Verify installation:**
```bash
brew --version
```

---

### Step 3: Install Build Dependencies

Install CMake and other required tools:

```bash
# Install CMake
brew install cmake pkg-config

# Install Rust
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env

# Install Python (if not already installed)
brew install python@3.11
```

---

### Step 4: Clone and Build Corepy

```bash
# Clone the repository
git clone https://github.com/ai-foundation-software/corepy.git
cd corepy

# Create a virtual environment
python3 -m venv .venv
source .venv/bin/activate

# Install Python dependencies
pip install --upgrade pip
pip install -r requirements-base.txt

# Build C++ kernels with Metal support
cd csrc
cmake -B build -DUSE_METAL=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
cd ..

# Install Corepy (editable mode)
pip install -e .
```

> [!IMPORTANT]
> The `-DUSE_METAL=ON` flag enables GPU acceleration via Apple's Metal framework. This is automatically optimized for your specific M-series chip.

---

## ✅ Verify GPU Support

After installation, verify that Corepy can detect your Apple Silicon GPU:

```python
import corepy as cp

# Check device information
device_info = cp.get_device_info()
print(f"GPU Count: {device_info.gpu_count}")
print(f"GPU Names: {device_info.gpu_names}")
print(f"Has GPU: {device_info.has_gpu}")
print(f"Platform: {device_info.platform}")
```

Expected output:
```
GPU Count: 1
GPU Names: ['Apple M1 Pro GPU']  # Or M2, M3, M4 depending on your chip
Has GPU: True
Platform: darwin
```

---

## 🧪 Test GPU Performance

Run a benchmark to confirm Metal acceleration is working:

```python
import corepy as cp
import numpy as np
import time

# Create large matrices
size = 4096
a = cp.Tensor(np.random.rand(size, size))
b = cp.Tensor(np.random.rand(size, size))

# Force GPU backend
cp.set_backend_policy('gpu')

# Benchmark matrix multiplication
start = time.time()
c = cp.matmul(a, b)
gpu_time = time.time() - start

print(f"GPU Time (Metal): {gpu_time:.4f} seconds")
print(cp.explain_last_dispatch())

# Compare with CPU
cp.set_backend_policy('cpu')
start = time.time()
c_cpu = cp.matmul(a, b)
cpu_time = time.time() - start

print(f"CPU Time: {cpu_time:.4f} seconds")
print(f"Speedup: {cpu_time/gpu_time:.2f}x")
```

---

## 🔧 Troubleshooting

### Issue 1: "xcrun: error: invalid active developer path"

**Solution:**
Reinstall Xcode Command Line Tools:
```bash
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
```

---

### Issue 2: CMake not found

**Solution:**
Ensure Homebrew is in your PATH:
```bash
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
source ~/.zprofile
brew install cmake
```

---

### Issue 3: "No GPU detected" at runtime

**Solution:**
1. Verify you built with `-DUSE_METAL=ON`
2. Check the build log for Metal framework errors
3. Rebuild from scratch:
   ```bash
   cd csrc
   rm -rf build
   cmake -B build -DUSE_METAL=ON
   cmake --build build -j$(sysctl -n hw.ncpu)
   ```

---

### Issue 4: Build fails with "unsupported architecture"

**Solution:**
Make sure you're building natively on Apple Silicon (not Rosetta):
```bash
# Check if running under Rosetta
uname -m  # Should show "arm64", not "x86_64"

# If showing x86_64, you're in Rosetta mode
# Use the native Terminal.app or iTerm2 without Rosetta
arch -arm64 zsh  # Start a native shell
```

---

## 🎯 Apple Silicon Performance Characteristics

| Chip | GPU Cores | Unified Memory | Recommended Matrix Size |
|:-----|:----------|:---------------|:------------------------|
| M1 | 7-8 cores | 8-16 GB | Up to 8192×8192 |
| M1 Pro | 14-16 cores | 16-32 GB | Up to 12288×12288 |
| M1 Max | 24-32 cores | 32-64 GB | Up to 16384×16384 |
| M1 Ultra | 48-64 cores | 64-128 GB | Up to 24576×24576 |
| M2 | 8-10 cores | 8-24 GB | Up to 8192×8192 |
| M2 Pro | 16-19 cores | 16-32 GB | Up to 12288×12288 |
| M2 Max | 30-38 cores | 32-96 GB | Up to 16384×16384 |
| M2 Ultra | 60-76 cores | 64-192 GB | Up to 32768×32768 |
| M3 | 10 cores | 8-24 GB | Up to 8192×8192 |
| M3 Pro | 14-18 cores | 18-36 GB | Up to 12288×12288 |
| M3 Max | 30-40 cores | 36-128 GB | Up to 16384×16384 |
| M4 | 10 cores | 16-32 GB | Up to 10240×10240 |

---

## 🎯 Environment Variables

Control GPU usage with environment variables:

```bash
# Force GPU backend (Metal)
export COREPY_BACKEND=gpu

# Force CPU backend
export COREPY_BACKEND=cpu

# Enable Metal performance statistics
export METAL_DEVICE_WRAPPER_TYPE=1
export METAL_DEBUG_ERROR_MODE=1
```

---

## 🚢 Deploying to Another Mac

To use Corepy on **another Apple Silicon Mac**:

### Option 1: Install from Source (Recommended)

1. Follow **Steps 1-4** above on the target Mac
2. Ensure macOS versions are compatible
3. Build natively (not under Rosetta)

### Option 2: Use Pre-built Wheels (Future)

> [!NOTE]
> In the future, Corepy will provide pre-built wheels for Apple Silicon. For now, building from source is recommended.

---

## 📚 Additional Resources

- [Apple Metal Documentation](https://developer.apple.com/metal/)
- [Corepy Architecture Documentation](../03_architecture/architecture.md)
- [Performance Tuning Guide](../02_core_concepts/performance_analysis.md)

---

## ℹ️ Notes

### Unified Memory Advantage

Apple Silicon uses **unified memory** shared between CPU and GPU. This provides:
- **Zero-copy transfers**: No need to copy data between CPU and GPU
- **Faster operations**: Reduced memory overhead
- **Efficient multitasking**: CPU and GPU can work on the same data simultaneously

### Performance Tips

1. **Use larger batch sizes**: Unified memory allows larger operations
2. **Mixed precision**: M3/M4 chips support hardware-accelerated FP16/BF16
3. **Memory pressure**: Monitor with Activity Monitor's "Memory Pressure" graph

---

## 🆚 CPU vs GPU Performance

On Apple Silicon, GPU acceleration provides:
- **2-5x speedup** for matrix operations (≥ 2048×2048)
- **3-8x speedup** for large batch processing
- **1.5-3x speedup** for element-wise operations

The unified memory architecture makes even small operations benefit from GPU acceleration!

---

For questions or issues, please open an issue on [GitHub](https://github.com/ai-foundation-software/corepy/issues).

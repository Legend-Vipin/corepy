# 🎮 GPU Setup - All Platforms Quick Reference

> Quick setup guide for Corepy GPU acceleration on Linux, macOS, and Windows

---

## 🐧 Linux (NVIDIA RTX)

### Prerequisites
- NVIDIA RTX GPU (2000/3000/4000/5000 series)
- Ubuntu 20.04+ or similar Linux distribution

### Quick Install Commands
```bash
# 1. Install NVIDIA drivers
sudo add-apt-repository ppa:graphics-drivers/ppa -y
sudo apt update && sudo ubuntu-drivers autoinstall
sudo reboot

# 2. Install CUDA 12.3
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt-get update && sudo apt-get install -y cuda-toolkit-12-3

# 3. Add CUDA to PATH
echo 'export PATH=/usr/local/cuda-12.3/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda-12.3/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc

# 4. Install build tools
sudo apt-get install -y build-essential cmake pkg-config
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source $HOME/.cargo/env

# 5. Build Corepy (choose your GPU architecture)
git clone https://github.com/ai-foundation-software/corepy.git
cd corepy
python3 -m venv .venv && source .venv/bin/activate
pip install --upgrade pip && pip install -r requirements-base.txt
cd csrc
# RTX 2050: 75 | RTX 3060: 86 | RTX 4090: 89
cmake -B build -DUSE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES=75
cmake --build build -j$(nproc)
cd .. && pip install -e .
```

**[Full Linux Guide](gpu_setup.md)**

---

## 🍎 macOS (Apple Silicon)

### Prerequisites
- Mac with M1/M2/M3/M4 chip
- macOS 12.0+ (macOS 13+ recommended)

### Quick Install Commands
```bash
# 1. Install Xcode Command Line Tools
xcode-select --install

# 2. Install Homebrew (if needed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zprofile
eval "$(/opt/homebrew/bin/brew shellenv)"

# 3. Install build tools
brew install cmake pkg-config python@3.11
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source $HOME/.cargo/env

# 4. Build Corepy with Metal support
git clone https://github.com/ai-foundation-software/corepy.git
cd corepy
python3 -m venv .venv && source .venv/bin/activate
pip install --upgrade pip && pip install -r requirements-base.txt
cd csrc
cmake -B build -DUSE_METAL=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
cd .. && pip install -e .
```

> **Note**: No GPU drivers or CUDA needed! Metal is built into macOS. ✨

**[Full macOS Guide](gpu_setup_macos.md)**

---

## 🪟 Windows (NVIDIA RTX)

### Prerequisites
- NVIDIA RTX GPU (2000/3000/4000/5000 series)
- Windows 10/11 (64-bit)
- Visual Studio 2022 with C++ Desktop Development

### Quick Install Steps

**1. Install NVIDIA Drivers**
- Download [GeForce Experience](https://www.nvidia.com/en-us/geforce/geforce-experience/)
- Install and update drivers
- Restart PC

**2. Install Visual Studio 2022**
- Download [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/)
- Select "Desktop development with C++"
- Install

**3. Install CUDA Toolkit 12.3**
- Download from [NVIDIA CUDA Downloads](https://developer.nvidia.com/cuda-downloads)
- Run installer, choose Custom installation
- Select CUDA Toolkit, Development, and Runtime

**4. Install CMake and Rust**
- Download [CMake](https://cmake.org/download/) (Windows x64 Installer)
  - ✅ Check "Add CMake to PATH"
- Download [rustup-init.exe](https://rustup.rs/) and run

**5. Build Corepy**

Open Command Prompt:
```cmd
git clone https://github.com/ai-foundation-software/corepy.git
cd corepy
python -m venv .venv
.venv\Scripts\activate
python -m pip install --upgrade pip
pip install -r requirements-base.txt
cd csrc

:: Choose GPU architecture: RTX 2050=75, RTX 3060=86, RTX 4090=89
cmake -B build -G "Visual Studio 17 2022" -A x64 ^
  -DUSE_CUDA=ON ^
  -DCMAKE_CUDA_ARCHITECTURES=75 ^
  -DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release
cd ..
pip install -e .
```

**[Full Windows Guide](gpu_setup_windows.md)**

---

## ✅ Verification (All Platforms)

After installation, test GPU detection:

```python
import corepy as cp

info = cp.get_device_info()
print(f"GPU Count: {info.gpu_count}")
print(f"GPU Names: {info.gpu_names}")
print(f"Has GPU: {info.has_gpu}")
```

---

## 📊 GPU Architecture Quick Reference

### Linux & Windows (NVIDIA CUDA)

| GPU Series | Models | Compute | CMake Flag |
|:-----------|:-------|:--------|:-----------|
| RTX 2000 | 2050, 2060, 2070, 2080 | 7.5 | `75` |
| RTX 3000 | 3060, 3070, 3080, 3090 | 8.6 | `86` |
| RTX 4000 | 4060, 4070, 4080, 4090 | 8.9 | `89` |
| RTX 5000 | 5090 | 9.0 | `90` |

### macOS (Apple Metal)

| Chip | GPU Cores | Unified Memory | Max Matrix Size |
|:-----|:----------|:---------------|:----------------|
| M1 | 7-8 | 8-16 GB | 8192×8192 |
| M1 Pro/Max | 14-32 | 16-64 GB | 12288-16384 |
| M2 | 8-10 | 8-24 GB | 8192×8192 |
| M2 Pro/Max | 16-38 | 16-96 GB | 12288-16384 |
| M3 | 10 | 8-24 GB | 8192×8192 |
| M3 Pro/Max | 14-40 | 18-128 GB | 12288-16384 |
| M4 | 10 | 16-32 GB | 10240×10240 |

---

## 🔧 Common Troubleshooting

| Issue | Linux | macOS | Windows |
|:------|:------|:------|:--------|
| GPU not detected | Check `nvidia-smi` | Verify Metal in Activity Monitor | Check `nvidia-smi` in CMD |
| Build fails | Install `build-essential cmake` | Run `xcode-select --install` | Use VS Developer Command Prompt |
| CUDA not found | Add to PATH in `~/.bashrc` | N/A (uses Metal) | Add CUDA to System PATH |
| Wrong architecture | Rebuild with correct `-DCMAKE_CUDA_ARCHITECTURES` | Automatic for all M-series | Rebuild with correct compute capability |

---

## 🎯 Environment Variables (All Platforms)

### Linux/macOS
```bash
export COREPY_BACKEND=gpu  # Force GPU
export COREPY_BACKEND=cpu  # Force CPU
```

### Windows
```cmd
set COREPY_BACKEND=gpu  # Force GPU
set COREPY_BACKEND=cpu  # Force CPU
```

---

## 📚 Platform-Specific Documentation

- **🐧 [Linux (NVIDIA RTX) - Full Guide](gpu_setup.md)**
- **🍎 [macOS (Apple Silicon) - Full Guide](gpu_setup_macos.md)**
- **🪟 [Windows (NVIDIA RTX) - Full Guide](gpu_setup_windows.md)**
- **📋 [Linux Quick Checklist](gpu_quick_reference.md)**

---

## 🌐 Platform Comparison

| Feature | Linux (NVIDIA) | macOS (Apple Silicon) | Windows (NVIDIA) |
|:--------|:--------------|:---------------------|:-----------------|
| **GPU Framework** | CUDA | Metal | CUDA |
| **Setup Complexity** | Medium | Easy | Medium-Hard |
| **Driver Install** | Manual | Built-in | GeForce Experience |
| **Performance** | Excellent | Excellent | Excellent |
| **Memory** | VRAM (4-24GB) | Unified (8-192GB) | VRAM (4-24GB) |
| **Best For** | Servers, Deep Learning | Development, Unified Memory | Gaming PCs, Workstations |

---

For detailed instructions, troubleshooting, and performance tips, see the platform-specific guides above.

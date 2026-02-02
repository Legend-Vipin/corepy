# Corepy
<h1 align="center">
<img src="assets/logo.svg" width="300">
</h1><br>

[![CI](https://github.com/ai-foundation-software/corepy/actions/workflows/ci.yml/badge.svg)](https://github.com/ai-foundation-software/corepy/actions/workflows/ci.yml)
[![Ruff](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/astral-sh/ruff/main/assets/badge/v2.json)](https://github.com/astral-sh/ruff)
[![uv](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/astral-sh/uv/main/assets/badge/v0.json)](https://github.com/astral-sh/uv)
[![Python](https://img.shields.io/badge/python-3.9%20|%203.10%20|%203.11%20|%203.12%20|%203.13-blue)](https://www.python.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
<!-- ![Corepy Logo](assets/logo.svg) -->

> **A high-performance Python foundation for data and AI.**
> *Correctness First. Device Aware. Built to Last.*

## 📖 What is Corepy?

Corepy is a library that helps you write fast and safe Python code for data processing and AI.

If you have ever:
- Waited too long for a Python loop to finish.
- Crashed your program because of a "Segmentation Fault" in a C extension.
- Struggled to get your code running on both a laptop (CPU) and a powerful server (GPU).

Corepy is designed to solve these problems. It combines the ease of **Python**, the raw speed of **C++**, and the safety of **Rust**.

### Key Features
- **🚀 Fast**: Uses C++ for heavy number crunching (SIMD optimized).
- **🛡️ Safe**: Uses Rust to manage memory and parallel tasks, preventing crashes.
- **🎮 GPU Accelerated**: Support for NVIDIA RTX GPUs (2000/3000/4000/5000 series).
- **💻 Everywhere**: Works on Linux, macOS (including Apple Silicon), and Windows.
- **🤖 Smart**: Automatically uses your hardware (like multiple CPU cores) without complex setup.

---

## 💻 Supported Platforms

Corepy works on all major operating systems with both **CPU** and **GPU** acceleration:

| Platform | Architecture | CPU Support | GPU Support | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Linux** | x86_64, aarch64 | ✅ Full | ✅ NVIDIA RTX (CUDA) | Ubuntu 20.04+ (primary), Debian, Fedora, RHEL |
| **macOS** | Apple Silicon (M1/M2/M3/M4) | ✅ Full | ✅ Metal Acceleration | Optimized for Apple's hardware with unified memory |
| **macOS** | Intel x86_64 | ✅ Full | ❌ CPU-only | Intel Macs supported (no GPU acceleration) |
| **Windows** | x86_64 | ✅ Full | ✅ NVIDIA RTX (CUDA) | Windows 10/11 with Visual Studio 2022 |

**GPU Support:**
- **NVIDIA RTX**: 2000, 3000, 4000, 5000 series (Linux & Windows)
- **Apple Silicon**: M1, M2, M3, M4 and Pro/Max/Ultra variants (macOS)

See [Platform Support Guide](docs/01_quickstart/platform_support.md) for detailed setup instructions.

---

## 🛠️ Installation

### Option 1: Install via pip (Recommended)
If you just want to use the library:

```bash
pip install corepy
```

### Option 2: Install from Source (For Developers)
If you want to contribute or change the code, see our [Installation Guide](docs/01_quickstart/install.md).

---

## ⚡ Quick Start

Here is a simple example showing how to load some data and process it safely.

```python
import corepy as cp

# 1. Load data efficiently (automatically uses parallel processing)
# This looks like normal Python, but it's powered by Rust under the hood.
data = cp.read_csv("data.csv")

# 2. Perform a calculation
# Corepy automatically selects the best way to run this on your CPU.
result = data.select("price").mean()

print(f"Average Price: {result}")
```


For more examples, see the [Usage Guide](docs/01_quickstart/usage.md).

## 🚀 Performance Profiling

Corepy includes a built-in zero-config profiler to help you optimize your code.

```python
import corepy as cp
cp.enable_profiling()
# ... run your code ...
print(cp.profile_report())
```

See the [Profiling Guide](docs/02_core_concepts/profiling.md) for full details.

---

## 📚 Documentation
- [**Platform Support Guide**](docs/01_quickstart/platform_support.md): CPU & GPU setup for Linux, macOS, and Windows.
- [**Installation Guide**](docs/01_quickstart/install.md): Detailed setup instructions.
- **GPU Setup Guides**:
  - [Linux (NVIDIA RTX 2000-5000)](docs/01_quickstart/gpu_setup.md) - CUDA setup for RTX GPUs
  - [macOS (Apple Silicon)](docs/01_quickstart/gpu_setup_macos.md) - Metal acceleration for M1/M2/M3/M4
  - [Windows (NVIDIA RTX)](docs/01_quickstart/gpu_setup_windows.md) - CUDA setup on Windows
  - [All Platforms Quick Reference](docs/01_quickstart/gpu_quick_reference.md) - Quick setup checklist
- [**Usage Guide**](docs/01_quickstart/usage.md): How to use Corepy for real work.
- [**Profiling Guide**](docs/02_core_concepts/profiling.md): Performance optimization guide.
- [**Architecture**](docs/03_architecture/architecture.md): Internal design and data flow.
- [**Roadmap**](docs/00_overview/roadmap.md): Future plans and milestones.
- [**Contributing**](docs/07_contributing/CONTRIBUTING.md): How to help build Corepy.

## 🤝 Stability
Corepy is currently in **Alpha** (v0.2.0). 
See [Roadmap](docs/00_overview/roadmap.md) for our path to v1.0.

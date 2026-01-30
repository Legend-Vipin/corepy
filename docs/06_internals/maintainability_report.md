# Corepy Maintainability & Audit Report - v0.2.0

## 1. Project Audit Summary
- **Type**: Hybrid Python/Rust/C++ Library
- **Architecture**: 3-layer execution model (Python UI -> Rust Scheduler -> C++ Kernel)
- **Status**: Alpha (v0.2.0)
- **Health**: 🟢 Good. Structure is modular and separation of concerns is clear.

## 2. Restructuring Actions Taken
| File / Directory | Old Location | New Location | Reason |
|------------------|--------------|--------------|--------|
| `PROFILING_GUIDE.md` | Root | `docs/profiling.md` | Standard docs location |
| `ARCHITECTURE_VISION.md` | Root | `docs/archive/` | Superseded by actual impl |
| `GAP_ANALYSIS...md` | Root | `docs/archive/` | Historic context |
| `test_released_api.py` | Root | `tests/integration/` | It is a test |
| `working_examples.py` | Root | `examples/` | It is an example |
| `csrc/` | `csrc/` | `csrc/` | Kept standard for PyTorch-like builds |

## 3. Directory Structure (Standardized)
```
📦 corepy/
 ┣ 📂 corepy/                 # Python source (Public API)
 ┃ ┣ 📂 backend/              # Device & Dispatch logic
 ┃ ┣ 📂 profiler/             # Profiling engine
 ┃ ┗ 📂 runtime/              # Execution pipeline
 ┣ 📂 csrc/                   # C++ Kernels (AVX/SIMD)
 ┣ 📂 rust/                   # Rust Runtime (Scheduler, Memory)
 ┣ 📂 bindings/               # FFI Glue
 ┣ 📂 docs/                   # Documentation
 ┃ ┣ 📂 archive/              # Historic design docs
 ┃ ┣ architecture.md          # Active design doc
 ┃ ┗ roadmap.md               # Future milestones
 ┣ 📂 tests/                  # Unit & Integration tests
 ┣ 📂 benchmarks/             # Performance scripts
 ┣ 📂 examples/               # User examples
 ┣ 📜 pyproject.toml          # Build config
 ┗ 📜 README.md               # Entry point
```

## 4. Next Steps
1.  **CI/CD**: Set up GitHub Actions to run `pytest` and `maturin build`.
2.  **Linting**: Configure `ruff` and `mypy` (deps already added).
3.  **API Stability**: Define public vs private API explicitly (`__all__`).

## 5. Risks
- **Split Brain**: Ensure `corepy-runtime` (Rust) version always matches `corepy` (Python) in `pyproject.toml`.
- **Packaging**: Complex mixed build (scikit-build + maturin) needs careful wheel verification.

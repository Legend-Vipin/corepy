# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.2.1] - 2026-01-30

### Added
- **Optimization**: CPU-optimized `matmul` dispatch in `corepy.tensor` using C++ backend.
- **Tutorials**: Comprehensive guides for profiling, optimization, and advanced usage in `tutorials/`.
- **Scripts**: Moved root housekeeping scripts to `benchmarks/` and `examples/` for cleaner structure.

### Infrastructure
- **Build System**: Modernized `setup_dev.sh` and CI workflows (`.github/workflows/`) to use `uv` for dependency management.
- **Cleanup**: Removed accidental inclusion of `rust/target` build artifacts from git history.

## [0.2.0] - 2026-01-04

### Added
- **Profiling System**:
  - Zero-config profiling with `cp.enable_profiling()`.
  - Context manager `ProfileContext` for targeted profiling.
  - Decorator `@profile_operation` for custom functions.
  - Automatic bottleneck detection and optimization recommendations.
  - Support for JSON, CSV, and Flamegraph export formats.
- Complete `tutorials/` series for learning Corepy.
- `Table` data container prototype.
- `Schema` definition system.
- `Pipeline` execution engine.

### Infrastructure (2026-01-30)
- **Documentation**: Added comprehensive documentation structure (docs/00-08)
- **Development Tools**:
  - Development setup guide (DEVELOPMENT.md)
  - Automated environment setup script (setup_dev.sh)
  - Directory README files (benchmarks/, examples/, tests/)
- **Build System**:
  - Migrated to uv for dependency management
  - Added maturin to development dependencies
  - Enhanced .gitignore with OS-specific and temporary file rules
- **Project Structure**: Archived historical reports to docs/archive/

### Fixed
- Memory safety issues with non-contiguous arrays
- Build script portability across platforms

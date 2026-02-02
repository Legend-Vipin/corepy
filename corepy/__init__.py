"""
Corepy: A unified, high-performance core runtime.
"""
from corepy import data, runtime, schema

from . import backend
from .ops import math as _math_ops  # Trigger registration
from .profiler import (
    ProfileContext,
    clear_profile,
    detect_bottlenecks,
    detect_regressions,
    disable_profiling,
    enable_profiling,
    export_profile,
    get_recommendations,
    profile_operation,
    profile_report,
)
from .tensor import Tensor

try:
    from ._corepy_cpp import add_one
except ImportError:
    # Fallback or warning if extension is not present (e.g. during dev without compile)
    def add_one(x: int) -> int:
        raise ImportError("C++ extension not loaded. Did you install with -v?")

try:
    from . import _corepy_rust
except ImportError:
    pass # Managed in usage sites

tensor = Tensor

__version__ = "0.2.1"

# Expose types and backend control
from .backend import (
    DataType,
    BackendPolicy,
    get_backend_policy,
    set_backend_policy,
    explain_last_dispatch
)

Float32 = DataType.FLOAT32
Float64 = DataType.FLOAT64
Int32 = DataType.INT32
Int64 = DataType.INT64
Bool = DataType.BOOL

__all__ = [
    "data", "schema", "runtime", "add_one", "Tensor", "tensor", "backend", "profiler",
    "enable_profiling", "disable_profiling", "clear_profile", "profile_report",
    "export_profile", "ProfileContext", "profile_operation", "detect_bottlenecks",
    "get_recommendations", "detect_regressions",
    "Float32", "Float64", "Int32", "Int64", "Bool", "DataType",
    "BackendPolicy", "get_backend_policy", "set_backend_policy", "explain_last_dispatch"
]

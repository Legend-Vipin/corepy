from typing import Any

from ..backend.dispatch import register_kernel
from ..backend.types import BackendType

# ============================================================================
# ARCHITECTURE COMPLIANCE NOTE:
# ============================================================================
# This file registers kernel stubs for the dispatch system.
# 
# According to the 3-layer architecture (execution_model.md):
# - Python Layer: Zero performance logic (INTENT ONLY)
# - Rust Layer: Validation, scheduling, memory management
# - C++ Layer: Pure execution (SIMD kernels)
#
# These functions should NEVER execute. They exist to:
# 1. Register operation names in the dispatch system
# 2. Raise errors if Rust/C++ dispatch fails
#
# If you see these errors during normal execution, it means the FFI
# dispatch is broken and needs to be fixed.
# ============================================================================

@register_kernel("add", BackendType.CPU)
def cpu_add(a: Any, b: Any) -> Any:
    """
    Element-wise addition stub.
    
    ARCHITECTURE VIOLATION: This should never execute!
    Execution path should be: Python → Rust → C++ (SIMD kernel)
    
    If you see this error, the Rust/C++ dispatch layer is not working.
    """
    raise NotImplementedError(
        "CPU add kernel should execute via Rust/C++ FFI, not Python. "
        "This is an architecture violation. Check lib.rs for tensor_add() function."
    )

@register_kernel("matmul", BackendType.CPU)
def cpu_matmul(a: Any, b: Any) -> Any:
    """
    Matrix multiplication stub.
    
    ARCHITECTURE VIOLATION: This should never execute!
    Execution path should be: Python → Rust → C++ (GEMM kernel)
    
    If you see this error, the Rust/C++ dispatch layer is not working.
    """
    raise NotImplementedError(
        "CPU matmul kernel should execute via Rust/C++ FFI, not Python. "
        "This is an architecture violation. Check lib.rs for tensor_matmul() function."
    )

@register_kernel("all", BackendType.CPU)
def cpu_all(a: Any) -> bool:
    """
    Reduction: all() operation stub.
    
    TEMPORARY FALLBACK: This executes in Python until Rust FFI is integrated.
    
    TODO: Once tensor.all() is updated to call _corepy_rust.tensor_all(),
          this should raise NotImplementedError like the others.
    """
    # Temporary Python fallback for tensor.all()
    # Will be removed once Python layer is updated to use raw pointer FFI
    if isinstance(a, list):
        return all(a)
    else:
        return bool(a)



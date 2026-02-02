from .backend import Backend, CPUBackend, GPUBackend
from .device import CPUDevice, Device, DeviceInfo, GPUDevice, detect_devices
from .reference import ReferenceBackend
from .selector import select_backend
from .session import Session, get_session
from .types import BackendType, DataType, OperationProperties, OperationType
from enum import IntEnum

class BackendPolicy(IntEnum):
    DEFAULT = 0
    OPENBLAS = 1
    BLAS = 2
    CUDA = 3

def set_backend_policy(policy: BackendPolicy):
    """Set the global CPU backend selection policy."""
    from corepy import _corepy_rust
    _corepy_rust.set_backend_policy(int(policy))

def get_backend_policy() -> BackendPolicy:
    """Get the current global CPU backend selection policy."""
    from corepy import _corepy_rust
    return BackendPolicy(_corepy_rust.get_backend_policy())

def explain_last_dispatch() -> str:
    """Returns a string explaining which backend was used for the last operation."""
    from corepy import _corepy_rust
    return _corepy_rust.explain_last_dispatch()

__all__ = [
    "BackendType",
    "OperationType",
    "OperationProperties",
    "DataType",
    "DeviceInfo",
    "BackendPolicy",
    "set_backend_policy",
    "get_backend_policy",
    "explain_last_dispatch",
    "BackendError",
    "DeviceNotFoundError",
    "OutOfMemoryError",
    "Device",
    "CPUDevice",
    "GPUDevice",
    "detect_devices",
    "Backend",
    "CPUBackend",
    "GPUBackend",
    "ReferenceBackend",
    "select_backend",
    "get_session",
    "Session",
]

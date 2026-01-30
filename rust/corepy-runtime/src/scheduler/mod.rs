// ============================================================================
// Scheduler: Work-Stealing Execution Engine
// ============================================================================
// This module contains the rayon-based scheduler for parallel execution.
//
// MODULES:
// - rayon_pool: Thread pool management and GIL-aware execution
// - arena: Thread-local memory arenas for temporary allocations

pub mod rayon_pool;
pub mod arena;

// Re-export commonly used functions


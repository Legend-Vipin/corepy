//! Profiler module for corepy
//!
//! This module implements the performance profiling system.

pub mod metrics;
pub mod core;

pub use self::core::{Profiler, ProfileScope, set_context};

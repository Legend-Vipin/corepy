# Tutorial 02: Intermediate Profiling

**Time to Complete**: 20 minutes  
**Difficulty**: Intermediate  
**Prerequisites**: Completed Tutorial 01 (Profiling Basics)

---

## What You'll Learn

In this tutorial, you'll learn advanced profiling techniques:
1. ✅ Profile specific code sections with context managers
2. ✅ Profile your own functions with decorators
3. ✅ Detect bottlenecks automatically
4. ✅ Get and apply optimization recommendations

---

## Why This Matters

**Beginner profiling** tracks everything globally. But in real projects:
- 🎯 You want to profile **specific sections** (just your ML training loop)
- 🎯 You want to profile **custom functions** (your data preprocessing pipeline)
- 🎯 You want **automatic recommendations** (don't analyze manually!)

This tutorial teaches you production-ready profiling techniques.

---

## Tutorial Structure

### 📝 [01_context_manager.py](01_context_manager.py)
Learn to profile specific code blocks using Python's `with` statement.

**Use Case**: You have a large application and only want to profile one section.

**Key Concepts**:
- `ProfileContext` context manager
- Named profiling sections
- Isolated metrics

---

### 📝 [02_custom_decorators.py](02_custom_decorators.py)
Learn to profile your own functions with the `@profile_operation` decorator.

**Use Case**: You have custom data processing functions to profile.

**Key Concepts**:
- `@profile_operation` decorator
- Function-level tracking
- Integration with existing code

---

### 📝 [03_bottleneck_detection.py](03_bottleneck_detection.py)
Learn how corepy automatically detects performance bottlenecks.

**Use Case**: Find slow operations without manual analysis.

**Key Concepts**:
- Automatic bottleneck detection
- Performance thresholds
- Anomaly detection

---

### 📝 [04_optimization_tips.py](04_optimization_tips.py)
Learn to get and apply automatic optimization recommendations.

**Use Case**: Get actionable advice to make your code faster.

**Key Concepts**:
- Recommendation engine
- Backend switching suggestions
- Batching opportunities
- Real before/after comparisons

---

## Quick Example

Profile just the important part of your code:

```python
import corepy as cp
from corepy.profiler import ProfileContext

# This won't be profiled
setup_data = expensive_io_operation()

# Only profile this critical section
with ProfileContext("training_loop"):
    for epoch in range(100):
        loss = train_step(data)

# Get targeted report for just the training loop
report = cp.profile_report(context="training_loop")
```

---

## Real-World Benefits

After this tutorial, you'll be able to:
- ⚡ **Find bottlenecks in minutes** instead of hours of guessing
- 🎯 **Profile production code** without tracking everything
- 🤖 **Get AI-powered suggestions** for optimization
- 📊 **Compare before/after** to verify improvements

---

## Next Steps

After completing this tutorial:
- 📚 Move to **Tutorial 03: Advanced Features**
- 🔥 Learn flamegraph analysis
- 🚀 Production monitoring techniques

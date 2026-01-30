# Tutorial 01: Profiling Basics

**Time to Complete**: 10 minutes  
**Difficulty**: Beginner  
**Prerequisites**: Basic Python knowledge, corepy installed

---

## What You'll Learn

In this tutorial, you'll learn:
1. ✅ How to enable performance profiling in corepy
2. ✅ How to generate your first performance report
3. ✅ How to interpret profiling metrics
4. ✅ How to identify slow operations

---

## Why This Matters

Without profiling, you're **blind** to performance issues:
- ❌ You don't know which operations are slow
- ❌ You can't measure the impact of optimizations
- ❌ You waste time optimizing the wrong code

With profiling, you get **instant insights**:
- ✅ See exactly where time is spent
- ✅ Get automatic optimization recommendations
- ✅ Make data-driven performance decisions

---

## Tutorial Structure

Work through these files in order:

### 📝 [01_enable_profiling.py](01_enable_profiling.py)
Learn how to turn profiling on and off with a single line of code.

**Key Concepts**: 
- `cp.enable_profiling()` - Start tracking operations
- `cp.disable_profiling()` - Stop tracking
- Zero-overhead when disabled

---

### 📝 [02_first_report.py](02_first_report.py)
Generate your first performance report and understand the output.

**Key Concepts**:
- `cp.profile_report()` - View metrics
- Operation counts and timing
- Backend information (CPU/GPU)

---

### 📝 [03_understanding_metrics.py](03_understanding_metrics.py)
Deep dive into what each metric means and how to use it.

**Key Concepts**:
- Average time per operation
- Percentage of total time
- Performance baselines
- Optimization recommendations

---

## Quick Start

If you're in a hurry, run this:

```python
import corepy as cp

# 1. Enable profiling
cp.enable_profiling()

# 2. Run your code
data = cp.tensor([1.0, 2.0, 3.0, 4.0, 5.0])
result = data * 2 + data

# 3. See the results
print(cp.profile_report())
```

Now work through the tutorials to understand the details!

---

## Next Steps

After completing this tutorial:
- 📚 Move to **Tutorial 02: Intermediate Usage**
- 🎯 Learn to profile specific code sections
- 🚀 Apply profiling to real projects

# Tutorial 03: Advanced Profiling Features

**Time to Complete**: 30 minutes  
**Difficulty**: Advanced  
**Prerequisites**: Completed Tutorials 01 & 02

---

## What You'll Learn

Master advanced profiling techniques used by performance engineers:
1. ✅ Generate and analyze flamegraphs
2. ✅ Export data to external tools (Speedscope, Chrome Tracing)
3. ✅ Set custom performance baselines
4. ✅ Use profiling in production safely

---

## Why This Matters

**Intermediate profiling** helps you find bottlenecks. But professionals need:
- 🔥 **Visual Analysis**: Flamegraphs show the big picture instantly
- 🔧 **Tool Integration**: Use industry-standard analysis tools
- 📊 **Baseline Tracking**: Detect regressions in CI/CD
- 🚀 **Production Monitoring**: Monitor live systems without impacting users

This tutorial teaches production-grade profiling.

---

## Tutorial Structure

### 📝 [01_flamegraph_analysis.py](01_flamegraph_analysis.py)
Generate and interpret flamegraph visualizations.

**Use Case**: Visualize complex call hierarchies at a glance.

**Key Concepts**:
- Flamegraph generation
- Reading flame graphs (width = time, height = call stack)
- Finding bottlenecks visually

**Tools**: Speedscope (https://speedscope.app)

---

### 📝 [02_export_integration.py](02_export_integration.py)
Export profiling data to external analysis tools.

**Use Case**: Integrate with your existing performance tooling.

**Key Concepts**:
- JSON export (general purpose)
- CSV export (spreadsheet analysis)
- Chrome Tracing format (chrome://tracing)
- Speedscope format (flamegraphs)

---

### 📝 [03_custom_baselines.py](03_custom_baselines.py)
Set performance targets and detect regressions.

**Use Case**: Ensure new code doesn't slow things down.

**Key Concepts**:
- Creating baselines from profiling data
- Regression detection in CI/CD
- Performance budgets
- Alerting on threshold violations

---

### 📝 [04_production_monitoring.py](04_production_monitoring.py)
Use profiling safely in production environments.

**Use Case**: Monitor real user performance without overhead.

**Key Concepts**:
- Sampling strategies (<1% overhead)
- Conditional profiling (only slow requests)
- Metrics aggregation
- Production alerting

---

## Real-World Workflow

```python
# 1. Development: Profile locally
cp.enable_profiling()
run_feature()
cp.export_profile("dev_profile.json", format="flamegraph")
# Open in speedscope.app → find bottlenecks

# 2. CI/CD: Verify no regressions
baseline = load_baseline("main_branch.json")
regressions = cp.detect_regressions(baseline)
assert len(regressions) == 0, "Performance regression detected!"

# 3. Production: Sample 1% of requests
if should_profile_request():  # 1% sampling
    with ProfileContext("api_request"):
        process_request()
    if total_time > SLA_THRESHOLD:
        alert_performance_team()
```

---

## Prerequisites

Before starting, understand:
- Basic profiling (Tutorial 01)
- Context managers and decorators (Tutorial 02)
- Performance analysis concepts

---

## Tools You'll Use

| Tool | Purpose | Installation |
|------|---------|--------------|
| **Speedscope** | Flamegraph visualization | https://speedscope.app (web-based, no install) |
| **Chrome** | Chrome Tracing viewer | chrome://tracing (built-in) |
| **spreadsheets** | CSV analysis | Excel, Google Sheets, etc. |

---

## Next Steps

After completing this tutorial:
- 📚 Move to **Tutorial 04: Case Studies**
- 🎯 Apply techniques to real problems
- 🚀 Implement in your production code

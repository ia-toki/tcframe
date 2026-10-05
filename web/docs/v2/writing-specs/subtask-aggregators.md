# Subtask Aggregators

An aggregator combines the verdicts of the test cases in one subtask into a single subtask verdict. Declare it inside the subtask, after `Points(...)`.

```cpp
void Subtask1() {
    Points(30);
    MinAggregator();
    CONS(valueOf(N).isBetween(1, 5));
}

void Subtask2() {
    Points(30);
    SumAggregator();
    CONS(valueOf(N).isBetween(1, 8));
}

void Subtask3() {
    Points(40);
    ThresholdAggregator(18);
}
```

If a subtask doesn't declare one, `tcframe` uses `min` when the problem has subtasks, and `sum` otherwise. This matches 1.x.

## The three aggregators

| Declaration | Meaning |
| --- | --- |
| `MinAggregator()` | Full points only if every test case is AC. A partial `OK <score>` lowers the points to the smallest score. Any other verdict gives 0 points. |
| `SumAggregator()` | Points are shared across the test cases. Each AC test case earns `points / N`, and an `OK <score>` test case earns its own score. Partial points are kept even when another test case fails. |
| `ThresholdAggregator(t)` | Full points if every test case passes. A test case passes when it is AC, or when it prints `OK x` with `x <= t`. `OK x` with `x > t` counts as WA. |

`ThresholdAggregator` is the IOI-style "resource limit" subtask. The solution's resource use is reported as `OK x`, and the subtask is passed when `x` is at most `t`.

## Verdict order

When a subtask has several verdicts, the worst one wins. From best to worst: `AC`, `OK`, `WA`, `RTE`, `TLE`, `ERR`.

## Checking the result

`tcframe make` writes the aggregator into `build/spec.yml`:

```yaml
subtasks:
  - points: 30
    aggregator:
      slug: min
  - points: 30
    aggregator:
      slug: sum
  - points: 40
    aggregator:
      slug: threshold
      args: 18
```

Judges read this file to apply the same rules. See [spec.yml](../distribution/spec-yml).

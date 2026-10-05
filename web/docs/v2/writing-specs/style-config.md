# Style Config

`StyleConfig()` chooses how solutions are run and how their output is compared. If you leave it out, the problem uses the batch style with the default diff scorer.

```cpp
void StyleConfig() {
    BatchEvaluator();   // default, can be left out
}
```

## Evaluators

| Macro | Use for | Solution | Expected `.out` |
| --- | --- | --- | --- |
| `BatchEvaluator()` | Normal problems: read input, print output | Run per test case | Required |
| `InteractiveEvaluator()` | Interactive problems | Run with a communicator | Not used (`NoOutput()` is implied) |
| `OutputOnlyEvaluator()` | Problems where contestants submit outputs | Not run; submitted outputs are compared | Required |
| `FunctionalEvaluator({"key", ...})` | Problems where contestants submit functions | Several source files, one per key | Required (or add `NoOutput()`) |

- `NoOutput()` declares that the problem has no expected `.out` files. Only the generator's inputs are produced.
- `OutputOnlyEvaluator()` and `FunctionalEvaluator(...)` are covered in [Output-only](../grading/output-only) and [Multi-file solutions](../solutions/multi-file).
- `InteractiveEvaluator()` is covered in [Interactive](../grading/interactive).

## Scorers

The scorer decides whether a solution's output is correct. Set it with `CustomScorer()` in `StyleConfig`, or use the float-tolerance macros. See [Scorers](../grading/scorers) and [Float tolerance](../grading/float-tolerance).

```cpp
void StyleConfig() {
    BatchEvaluator();
    AbsoluteFloatToleranceScorer(6);   // accept absolute error up to 1e-6
}
```

## Example: output-only problem

```cpp
void StyleConfig() {
    OutputOnlyEvaluator();
}
```

The spec is otherwise the same as a batch problem. Only the way solutions are graded changes.

## Notes

- Macros are called in `StyleConfig()`, not in `GradingConfig()`.
- `CustomScorer()` turns on the scorer in the package. The scorer program is found and compiled automatically. See [Scorers](../grading/scorers).

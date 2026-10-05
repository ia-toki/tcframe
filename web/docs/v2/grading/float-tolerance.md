# Float Tolerance

Many problems accept an answer that is close enough to the expected value, for example a real number with an error up to `1e-6`. Instead of writing a custom scorer, use one of the float macros in `StyleConfig`.

```cpp
void StyleConfig() {
    BatchEvaluator();
    AbsoluteFloatToleranceScorer(6);   // tolerance 1e-6
}
```

## The macros

| Macro | Accepts a pair of numbers when |
| --- | --- |
| `AbsoluteFloatToleranceScorer(k)` | `\|a - b\| <= 10^-k` |
| `RelativeFloatToleranceScorer(k)` | `\|a - b\| <= 10^-k * max(\|a\|, \|b\|)` |
| `FloatToleranceScorer(k)` | Either of the two above holds |

`k` is an exponent, so `6` means `10^-6`.

## How output is compared

- Each output is split into whitespace-separated tokens. Line breaks don't matter.
- Tokens that are numbers are compared with the tolerance.
- Other tokens must match exactly.
- `nan` and `inf` are not numbers in this comparison.

`spec.yml` records the tolerance so judges can apply the same rule:

```yaml
helpers:
  scorer:
    additional_args: "float_absolute_tolerance 1e-06"
```

## Samples

`make` checks the sample outputs with the same tolerance as grading, so a sample such as `1.5` is accepted when the reference prints `1.500000`.

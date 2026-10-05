# spec.yml

`spec.yml` is the grading configuration for the judge. `tcframe make` writes it to `build/spec.yml`, from the spec program, so the judge doesn't need to run `spec.cpp`.

## Example

```yaml
slug: aplusb
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
evaluator:
  slug: batch
  solution_keys: [source]
  tc_output_present: true
helpers:
  scorer:
    additional_args: "float_absolute_tolerance 1e-06"
limits:
  time: 2000
  memory: 65536
```

## Fields

| Key | Meaning |
| --- | --- |
| `slug` | Problem name, taken from the package folder name |
| `subtasks` | One entry per subtask, in order. Empty for problems without subtasks. |
| `subtasks[].points` | Points for the subtask |
| `subtasks[].aggregator` | How the subtask's test case verdicts combine. See [Subtask aggregators](../writing-specs/subtask-aggregators). |
| `evaluator.slug` | `batch`, `interactive`, `output_only`, or `functional` |
| `evaluator.solution_keys` | Source keys a solution must provide. `[source]` for normal problems. |
| `evaluator.tc_output_present` | `true` when the `.out` files exist |
| `helpers` | Scorer or communicator settings. `additional_args` are the options for the default scorer. |
| `limits.time` | Time limit, in **milliseconds** |
| `limits.memory` | Memory limit, in **kilobytes** |

## Notes

- The limits come from `TimeLimit(...)` and `MemoryLimit(...)`, which take seconds and megabytes. `spec.yml` stores them in the units judges usually expect.
- Keys that don't apply are left out. For example, `subtasks` is `[]` when the problem has no subtasks.

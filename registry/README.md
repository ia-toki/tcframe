# tcframe registry

Default programs and configs that a problem package falls back to. Lookup order
(`cli/tcframe_cli/paths.py`): `$TCFRAME_HOME/registry`, then `~/.tcframe/registry`.
Layout:

```
registry/
  aggregators/{min,sum,threshold}/run     subtask aggregators (directory programs)
  helpers/scorer/compare/run              default scorer
  helpers/scorer/defaults.yml             slug: compare
  evaluators/{batch,interactive,output_only,functional}/evaluator.yml
  evaluators/functional/{build,run}_{cpp,pascal}  # custom build/run scripts (T6.3)
  # functional also needs manager/<family>/grader.[ext] in the package
```

## Functional evaluator and build/run scripts

Functional solutions are several source files, one per solution key. They are built
into one program, `./__tcframe_functional`, which is then run per test case.

- `build_[family] <solution-file>... <manager-dir>`: builds the solution files. The
  manager dir is the last argument (the helper args come after the solution files).
  It must leave the program at `./__tcframe_functional`.
- `run_[family] <input-file> <manager-dir>`: runs the program on one test case. Its
  stdout is the solution output, which the scorer then compares. (The RFC says the
  run script writes the verdict; this registry keeps the scorer path, because
  generation needs the `.out` file.)

Shipped: `cpp` (g++ with `manager/cpp/grader.cpp`) and `pascal` (FPC with
`manager/pascal/grader.pas`, each solution file a unit named like the file). The
scripts run from the working directory and need an `--evaluator-dir` that points to
this folder, plus `--solution-family=<family>`. Without them the built-in C++ build
is used.

Helpers in `evaluator.yml` are read by `cli/tcframe_cli/evaluators.py`: a program
helper is a package file or directory, falling back to the registry default when
optional; a files helper is a package directory.

## Aggregators

A program in a package named `aggregator_<subtasks>` (or `aggregator`) overrides
the registry one. Each registry aggregator is a directory with an executable
`run` script (POSIX sh + awk, no runtime dependencies).

- argv: `$1` subtask points, then the aggregator's own arguments (`threshold`: `$2` = t).
- stdin: `N`, then N test case verdict lines (`AC`, `WA`, `OK <value>`, `RTE`, `TLE`, `ERR`).
- stdout: one subtask verdict line.

| Aggregator | Extra arg | Output |
|---|---|---|
| `min` | — | `AC`, `OK <points>`, or the worst verdict. Points: full, lowered to the smallest OK score; any non-AC/OK verdict gives 0. |
| `sum` | — | `AC`, `OK <points>`, or `<VERDICT> <points>` for WA/RTE/TLE/ERR. Points: AC = points/N, OK = its own score; kept under any verdict, as in 1.x. |
| `threshold` | `t` | `AC` if every test case passes (AC, or `OK x` with `x <= t`), else the worst failing verdict. `OK x` with `x > t` counts as WA. |

Verdict order (worst last): AC < OK < WA < RTE < TLE < ERR, matching
`include/tcframe/runner/verdict/Verdict.hpp`.

## Scorer

`helpers/scorer/compare/run` follows the 1.x CustomScorer protocol:
argv `input expected contestant`; stdout is `AC` or `WA`; on WA the first 10
diff lines go to stderr.

Options come before the three positional args:

| Option | Effect |
|---|---|
| `ignore_whitespace` | exact diff becomes `diff -w` |
| `float_absolute_tolerance EPS` | numeric tokens pass if `\|a-b\| <= EPS` |
| `float_relative_tolerance EPS` | numeric tokens pass if `\|a-b\| <= EPS * max(\|a\|,\|b\|)` |

With a float option, output is compared token by token (line layout ignored);
non-numeric tokens must match exactly. With both float options, a pair passes if
either holds. The C++ `FloatToleranceScorer` implements the same rules.

`helpers/scorer/defaults.yml` names the scorer used when a package has no
`scorer` program (`slug: compare`).

## Evaluators

`evaluators/<name>/evaluator.yml` declares what a package must provide:

- `custom_solution_keys`: whether the evaluator takes solution keys other than `source`.
- `tc_output`: `required`, `not_required`, or `optional` (default).
- `helpers`: list of `{ slug, type, optional }`. `type: program` is built once
  before evaluation; `type: files` is a directory.

`output_only` takes the submitted outputs as its solution argument: a directory with
one file per test case, named like the expected output (`<name>.out`). No solution
is run when grading; the scorer compares each submitted output with the expected
one. A missing submitted output is WA.

The Functional evaluator's build and run scripts are described above (T6.3).

# Package Format

TCFrame 2.0 works with two package formats.

## Source package

A source package is what a problem setter edits. The directory name is the problem slug.

```
mypkg/
  spec.cpp                  the spec program
  metadata.yml              optional; copied verbatim into the packages
  solutions/
    ref/solution.cpp        the reference solution (exactly one file)
    ac/                     solutions that must get AC
    wa/                     solutions that must get WA
    tle/                    solutions that must get TLE
    rte/                    solutions that must get RTE
    ok-75/                  solutions that must get OK with score 75
    failed/                 solutions that must get WA, TLE, or RTE
  scorer.cpp | scorer/      optional custom scorer
  languages/*.yml           optional language definitions
```

### Solutions and expected verdicts

Each directory under `solutions/` (other than `ref/`) names the verdict that every file inside it must get:

| Directory | Expected |
| --- | --- |
| `ref/` | AC. Exactly one file. It generates the expected outputs. |
| `ac/` | AC |
| `wa/` | WA |
| `tle/` | TLE |
| `rte/` | RTE |
| `ok/` | OK, any score |
| `ok-N/` | OK with score N |
| `tle-N/` | TLE with score N, for a solution that gets partial points before timing out |
| `failed/` | WA, TLE, or RTE |

Each file is one solution. A solution with several source files (for example a function-style problem) is a directory instead.

Scores are absolute point values, the same as the points given in `spec.cpp`.

Run the checks with `tcframe test`.

## Distribution package

A distribution package is what an online judge consumes. `tcframe package` builds it: it contains everything in the source package, plus the generated files below.

```
mypkg/
  ...               everything from the source package
  spec.yml          grading configuration (subtasks, evaluator, limits, helpers)
  tc/               generated test cases: <name>.in and <name>.out
  validator         validates a test case input
```

`spec.yml` is written by the compiled spec program. It is the only grading configuration the judge reads, so judges do not need to run `spec.cpp`.

The `validator` program reads one test case on stdin. It prints the number of valid subtasks, then their IDs in ascending order, one per line. Input that breaks the constraints prints an error to stderr and exits with status 1. `tcframe make` runs the validator on every generated `.in` file and fails if any is rejected.

## Build outputs

Everything is written into the build directory (`./build` by default):

| File | Contents |
| --- | --- |
| `build/spec` | The compiled spec program |
| `build/spec.yml` | Grading configuration |
| `build/tc/` | Generated test cases |
| `build/validator` | The validator |
| `build/<slug>-source.zip` | The source package |
| `build/<slug>.zip` | The distribution package |

The 1.x `build/` layout is different. Do not rely on 1.x paths in 2.0 scripts.

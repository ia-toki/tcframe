# Structure

Monorepo root: the C++ framework lives at the top level; the Python CLI, registry,
and docs site are self-contained subfolders.

## Top-level layout

```
tcframe-v2/
  include/tcframe/      C++ header-only library (the framework itself)
  src/tcframe/          C++ source entry (runner.cpp, compiled per-spec)
  test/                 C++ tests: unit/, integration/, ete/
  scripts/tcframe       Shim: runs the Python CLI (cli/) with TCFRAME_HOME set
  cli/                  Python orchestration CLI (tcframe_cli, stdlib only)
  registry/             Default evaluators, helpers, aggregators (installed via TCFRAME_HOME)
  languages/            Language configs (*.yml) for automatic compilation
  templates/            `tcframe new` spec templates (batch, batch-subtasks, ...)
  examples/             Migrated v2 source packages (aplusb, aplusb_multi, distinct)
  web/                  Documentation site (Node/Yarn, deployed to tcframe.toki.id)
  CMakeLists.txt        CMake build for the C++ test suites
  codecov.yml           Coverage config
  .github/workflows/    CI (main.yml: ci + deploy-web)
  README.md             Project overview
  [RFC] tcframe 2.0 - NEW.md   Design proposal for the 2.0 direction
  LICENSE.txt           MIT license
```

## C++ library layout (`include/tcframe/`)

Headers are organized by domain, and tests under `test/` mirror this tree
one-to-one. Key areas:

- `spec/` — the user-facing spec model:
  - `core/` — `BaseProblemSpec`, `BaseTestSpec`, `Magic` (macro machinery).
  - `io/` — IO segments and manipulators (`LINE`, `LINES`, `GRID`, raw variants).
  - `constraint/` — constraints and subtasks.
  - `variable/` — scalar/vector/matrix variable handling and formatting.
  - `testcase/` — test suites, groups, sample/official cases.
  - `verifier/` — constraint verification.
  - `config/`, `random/` — grading/style config and the random helper.
- `runner/` — orchestration that turns a spec into generated/graded output:
  - `core/` — `Runner`, args parsing.
  - `generator/` — test case generation.
  - `grader/`, `aggregator/`, `evaluator/` — grading, subtask aggregation,
    evaluation styles (batch/interactive, scorers, communicators).
  - `client/`, `logger/`, `os/`, `verdict/` — supporting infrastructure.
- `driver/`, `validator/`, `exception/`, `util/` — cross-cutting helpers.

Umbrella headers (e.g. `spec.hpp`, `runner.hpp`, `driver.hpp`) re-export a
subtree so a spec can include a single header per area.

## Python CLI layout (`cli/`)

```
cli/
  tcframe_cli/
    cli.py            Subcommand dispatch (new, build, make, test, grade, package, version)
    build.py          Compile spec.cpp, emit spec.yml, generate tc/
    compiler.py       Program compiler (by extension, or build/run scripts)
    languages.py      Language config model + lookup (languages/*.yml)
    solutions.py      Parse solutions/{ref,ac,wa,tle,rte,ok-N,...}
    grading.py        Compile + grade each solution
    checker.py        Actual vs expected verdict matcher (test command)
    package.py        Source + distribution zips
    paths.py          TCFRAME_HOME / ~/.tcframe resolution
    simple_yaml.py    Zero-dep YAML subset reader
  tests/              pytest suite
  pyproject.toml      Packaging (hatchling), console script `tcframe`
```

## Conventions

- **Mirror the tree.** When adding a C++ header under `include/tcframe/<area>/`,
  add its test under `test/unit/tcframe/<area>/` and register both in
  `CMakeLists.txt` (the `INCLUDE` and `TEST_*` lists are explicit, not globbed).
- **Mocks** are named `Mock*.hpp` and sit next to the tests that use them.
- **The Python CLI has zero runtime dependencies** — keep it that way;
  prefer the standard library.
- **New generated/package formats** should follow the directions in the
  tcframe 2.0 RFC rather than inventing ad-hoc layouts.
```

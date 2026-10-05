# Product

## What this is

TCFrame is a framework for generating test cases for competitive programming
problems. Problem setters describe a problem's input/output format, constraints,
and test cases in a structured spec. TCFrame then produces the actual `.in` and
`.out` files, running the official solution to produce expected output, and
verifying every generated case against the declared constraints.

The project has two parts in this repo:

- **tcframe (C++)** — the header-only C++ framework. A spec is a `spec.cpp` file
  compiled into a `runner` program. This is the framework documented at
  https://tcframe.toki.id.
- **tcframe CLI (Python)** — the orchestration CLI in `cli/` (stdlib only). It
  compiles the spec, solutions, and helpers, then drives the C++ runner to
  produce `spec.yml`, `tc/`, `test` (solution verdict checks), and `package`
  zips. Specs are always C++; there is no Python-spec product.

The 2.0 direction, `[RFC] tcframe 2.0 - NEW.md`, with the phased plan in
`SPEC.md`, adds a Python CLI runner, pluggable evaluation and aggregation styles,
and standardized source/distribution package formats consumable by online
judges.

## Who it is for

Problem writers preparing problems for programming contests and online judges
(the project originates from ia-toki / TOKI and targets ecosystems like TLX).

## Why it exists

Writing test cases by hand is error-prone and tedious. Constraints change often
during problem development. TCFrame makes test generation:

- **Correct** — every case is checked against declared constraints, so invalid
  cases are caught with a clear error instead of shipping silently.
- **Repeatable** — a single small generator file replaces shipping large
  `testcases.zip` archives; regenerate whenever constraints change.
- **Consistent** — a shared format lets multiple setters collaborate on the same
  problem, with correct file naming and solution execution handled for them.

## Core capabilities

- Input/output format declaration via a rich set of format macros
  (`LINE`, `LINES`, `GRID`, `EMPTY_LINE`, raw variants).
- Constraint declaration with subtask support (ICPC-style and IOI-style
  problems with subtasks and points).
- Sample and official test case declaration, with a built-in random helper.
- Batch and interactive problem styles.
- Local grading against generated test cases with time and memory limits.

## Supported platforms

- C++ framework: Linux and macOS only (Windows unsupported); requires a GCC with
  C++17 support.
- Python CLI: requires Python >= 3.10 and a GCC with C++17 (it runs the C++
  toolchain). It is POSIX-only in practice like the C++ framework; interactive
  grading on Windows is not supported.

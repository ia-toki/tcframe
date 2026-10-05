# TCFrame 2.0 Overview

TCFrame 2.0 keeps the C++ spec format from 1.x and turns a problem into a **problem package**, managed by a single command: `tcframe`. The CLI compiles the spec, the solutions, and any helper programs automatically, checks that each solution gets the verdict you expect, and packages the problem for online judges.

The 1.x pages in this site still describe the 1.x workflow. Everything in this section is for 2.0.

## What changed from 1.x

- **Spec files are unchanged.** `spec.cpp` uses the same `InputFormat`, `Constraints`, `Subtask`, `TestGroup`, and `CASE` API as before.
- **No manual compilation.** Solutions, scorers, and helpers are compiled by extension (`solution.cpp` is built with `g++`). Languages are defined in `*.yml` files.
- **Problem packages.** A source package has a fixed layout (`spec.cpp`, `solutions/`, optional `metadata.yml`). A distribution package adds `spec.yml`, the generated `tc/` directory, and a `validator`.
- **Verdict checks.** `tcframe test` grades every file under `solutions/` and fails when a verdict does not match its directory name.

## Requirements

- Linux or macOS
- Python 3.10 or newer (the CLI uses only the standard library)
- GCC with C++17 support

## Setup

Clone or download TCFrame, then set `TCFRAME_HOME` to the checkout, as described in [Installation](../installation). The `tcframe` command is `scripts/tcframe` in that checkout:

```sh
export TCFRAME_HOME=~/tcframe
alias tcframe=$TCFRAME_HOME/scripts/tcframe
```

## Quick start

Create an empty problem directory, then:

```sh
tcframe new --template=batch                          # writes ./spec.cpp
mkdir -p solutions/ref
# write solutions/ref/solution.cpp (the reference solution)
tcframe make --solution=solutions/ref/solution.cpp    # build/spec.yml and build/tc/
tcframe test                                          # check every solutions/ entry
tcframe package --solution=solutions/ref/solution.cpp # build/<slug>-source.zip and build/<slug>.zip
```

`make` compiles `spec.cpp` into `build/spec`, runs it to generate the test cases, and writes `build/spec.yml`. `make`, `build`, `package`, and `grade` take `--solution`, which defaults to `./solution`. `test` finds the reference solution in `solutions/ref/` itself.

For a complete package, see [`examples/aplusb`](https://github.com/ia-toki/tcframe/tree/master/examples/aplusb) (batch), [`examples/aplusb_multi`](https://github.com/ia-toki/tcframe/tree/master/examples/aplusb_multi) (multiple test cases), and [`examples/distinct`](https://github.com/ia-toki/tcframe/tree/master/examples/distinct) (subtasks, with solutions that get AC, TLE, and RTE).

## Next steps

- [Commands](./commands): every subcommand and its options.
- [Package format](./package-format): the source and distribution layouts, verdict directories, and generated files.

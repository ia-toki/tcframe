# Commands

Run every command from the problem directory. Set `TCFRAME_HOME` first (see [Overview](./overview#setup)).

| Command | What it does |
| --- | --- |
| `tcframe new [--template=<t>]` | Writes `./spec.cpp` from a template. Default template: `batch`. Refuses to overwrite an existing `spec.cpp`. |
| `tcframe build [options]` | Compiles `spec.cpp` and the reference solution, then generates the test cases. |
| `tcframe make [options]` | Runs `build`, and writes `spec.yml` next to `tc/` in the build directory. |
| `tcframe test [--build-dir=<dir>]` | Runs `make`, grades every entry in `solutions/`, and compares each result with the verdict expected for its directory. Exits non-zero on any mismatch, so it fits CI. |
| `tcframe grade --solution=<cmd> [options]` | Grades one solution against the built test cases. Requires a previous `make`. |
| `tcframe package [options]` | Runs `make`, then writes the source and distribution zips. |
| `tcframe version` | Prints the TCFrame version. |

## Templates

| Template | Use for |
| --- | --- |
| `batch` | A single-answer problem |
| `batch-subtasks` | A problem with subtasks and points |
| `interactive` | An interactive problem (grade it with `--communicator=<cmd>`) |
| `interactive-subtasks` | An interactive problem with subtasks |

## Options

| Option | Commands | Meaning |
| --- | --- | --- |
| `--build-dir=<dir>` | build, make, test, package, grade | Build directory. Default: `./build`. |
| `--solution=<cmd>` | build, make, package, grade | The solution to grade or to generate outputs with. A source file such as `solutions/ref/solution.cpp` is compiled automatically. Default: `./solution`. |
| `--scorer=<cmd>` | build, make, test, package, grade | A custom scorer, for when the default diff comparison is not enough. A source file (for example `scorer.cpp`) is compiled automatically. |
| `--communicator=<cmd>` | build, make, test, package, grade | The communicator program for interactive problems. A source file (for example `communicator.cpp`) is compiled automatically. |

`grade` also accepts `--brief`, `--time-limit` / `--no-time-limit`, and `--memory-limit` / `--no-memory-limit`, as in 1.x.

## Environment

| Variable | Meaning |
| --- | --- |
| `TCFRAME_HOME` | The TCFrame checkout or install. Holds the C++ headers, the registry, and the templates. |
| `TCFRAME_PYTHON` | Optional. Python interpreter used by `scripts/tcframe`. Default: `python3`. |

Compiler flags can be passed through `TCFRAME_CXX_FLAGS`, as in 1.x.

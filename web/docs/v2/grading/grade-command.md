# Grading One Solution

`tcframe grade` runs one solution against the generated test cases and prints the result. Use it for a quick check of a single program, or for a submitted output directory (see [Output-only](./output-only)).

`tcframe test` is the tool for checking a whole package. Use `grade` when you want to run one program and see each test case.

## Before you start

1. Build the test cases once: `tcframe make --solution=solutions/ref/solution.cpp`.
2. Make sure the program you grade is **compiled**. `grade` needs an executable, not a source file.

```
tcframe: grade error: solution unavailable: solution '.../solutions/ref/solution.cpp' exists but is not executable.
```

Compile it first, then pass the executable:

```sh
g++ -std=c++17 -O2 -o my_sol my_sol.cpp
tcframe grade --solution=./my_sol
```

## Command

```sh
tcframe grade --solution=./my_sol [options]
```

| Option | Meaning |
| --- | --- |
| `--solution=<cmd>` | The program to run. Default: `./solution`. |
| `--build-dir=<dir>` | Build directory from `make`. Default: `./build`. |
| `--brief` | Print a shorter summary (overall and per-subtask verdicts). |
| `--time-limit=<s>` | Time limit in **seconds**. Default: the limit from the spec. |
| `--no-time-limit` | Turn the time limit off. |
| `--memory-limit=<mb>` | Memory limit in **MB**. Default: the limit from the spec. |
| `--no-memory-limit` | Turn the memory limit off. |
| `--scorer=<cmd>` | Custom scorer, see [Scorers](./scorers). |
| `--communicator=<cmd>` | Communicator for interactive problems, see [Interactive](./interactive). |

## Output

Each test case gets one line, grouped by sample and official cases:

```
[ OFFICIAL TEST CASES ]
  oo_1: Accepted
  oo_2: Wrong Answer
    * scorer: Submitted output not found: .../oo_2.out

[ VERDICT ]
  Wrong Answer [33.33]
```

The last line is the overall verdict with the points it earned.

## Exit code

`grade` exits with 0 when grading finished, even for `Wrong Answer`. The verdict is in the output, not the exit code. To fail a CI job, use [`tcframe test`](../commands) instead, which exits non-zero on any mismatch.

# Solutions Folder

The `solutions/` folder holds every program that `tcframe test` checks. Each folder name is the verdict its contents must get.

```
mypkg/
  solutions/
    ref/solution.cpp        reference solution (exactly one file)
    ac/ac1.cpp              must get AC
    wa/wa1.cpp              must get WA
    tle/tle1.cpp            must get TLE
    rte/rte1.cpp            must get RTE
    ok/ok1.cpp              must get OK (any score)
    ok-75/ok75.cpp          must get OK with 75 points
    tle-30/tle30.cpp        must get TLE with 30 points
    failed/f1.cpp           must get WA, TLE, or RTE
```

## Rules

- **`ref/`** holds exactly one entry, the reference solution. It must be correct (AC). `tcframe` uses it to generate the expected `.out` files.
- **Other folders** can hold any number of files. Every file in a folder is checked against that folder's verdict.
- **Names after the verdict** are case-sensitive and lowercase: `ac`, `wa`, `tle`, `rte`, `ok`, `failed`.
- **`ok-N` and `tle-N`** add an expected score. Use `tle-N` when a solution scores partial points and then times out, as with a subtask that is passed and then a subtask that times out.
- **Scores** are absolute points, the same units as `Points(...)` in `spec.cpp`.

## Running the checks

```sh
tcframe test
```

`test` compiles every file, grades it, and checks the verdict. See [Commands](../commands) for the options and [Compiling solutions](./compiling) for how files are built.

## Multi-file solutions

A folder can contain a directory instead of a file, for problems where each solution is several source files. See [Multi-file solutions](./multi-file).

## Output-only

Output-only problems don't use `solutions/`. Submitted outputs are graded with `tcframe grade`, as described in [Output-only](../grading/output-only).

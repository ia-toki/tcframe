# Troubleshooting

Common errors in TCFrame 2.0 and what to do about them.

## `option --spec-file unknown` or the wrong binary is used

Your `TCFRAME_HOME` points at an old checkout. The compiled spec then uses old headers.

Check the value:

```sh
echo $TCFRAME_HOME
```

Set it to your current checkout, or unset it so `scripts/tcframe` uses the checkout it lives in:

```sh
export TCFRAME_HOME=~/tcframe
```

## `reference solution unavailable`

`make` and `test` need the reference solution. Either the `--solution` path is wrong, or `solutions/ref/` doesn't exist.

```sh
ls solutions/ref
tcframe make --solution=solutions/ref/solution.cpp
```

## `reference solution failed to compile`

The reference doesn't build. Compile it by hand to see the full error:

```sh
g++ -std=c++17 -O2 -o /tmp/check solutions/ref/solution.cpp
```

## `solution unavailable ... is not executable`

`grade` needs a compiled program, not a source file. Compile first, then pass the executable. See [Grading one solution](./grading/grade-command).

## `no compiled spec at ...; run 'tcframe build' first`

`grade` needs a previous build. Run `make` first.

## `ERROR ... has no file for solution key 'encoder'`

A multi-file solution folder is missing one of its key files. Add the file, named after the key. See [Multi-file solutions](./solutions/multi-file).

## `Submitted output not found: ... .out`

An output-only submission is missing a file. Each test case needs `<test-case-name>.out`. See [Output-only](./grading/output-only).

## `Sample test case output does not match with actual output`

The sample's expected output doesn't match the reference solution's output during `make`. Check the sample against the solution's exact output format. If the difference is only in floating-point precision, declare a float scorer in `StyleConfig`. See [Float tolerance](./grading/float-tolerance).

## `ERROR  <folder>/<file>` in the `test` output

The solution couldn't be built or run, so it has no verdict. The line after it gives the reason, most often a compile error. Compile the file by hand to see the full message. See [Compiling solutions](./solutions/compiling).

## `tcframe test` fails on a solution but the verdict looks right

Check the folder name. A file in `wa/` must produce `WA`, and a file in `tle-30/` must produce `TLE` with 30 points. See [Solutions folder](./solutions/layout).

## Interactive problem: `ERR` on the reference

Pass the communicator with `--communicator=communicator.cpp`. The source is compiled for you. See [Interactive](./grading/interactive).

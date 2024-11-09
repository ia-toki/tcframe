# Runner program

A runner is the compiled version of a spec file, and is capable of two things:

## Test case generation

```
./runner [options]
```

- `--output=<dir>`

    The output directory to which the test cases will be generated. Default: `tc`.

- `--solution=<command>`

    The solution command to use for generating output files. Default: `./solution`.

- `--scorer=<command>`

    The custom scorer command to use for checking sample output strings in problem spec class. Default: `./scorer`.

- `--seed=<seed>`

    The seed for random number generator `rnd` in the test spec. Default: `0`.


## Local grading

```
./runner grade [options]
```

- `--output=<dir>`

    The output directory from which the generated test cases will be read. Default: `tc`.

- `--solution=<command>`

    The solution command to grade. Default: `./solution`.

- `--scorer=<command>`

    The custom scorer command to use. Default: `./scorer`.

- `--communicator=<command>`

    The communicator command to use. Default: `./communicator`.

- `--time-limit=<time-limit-in-seconds>`

    Overrides the time limit specified by `TimeLimit()` in grading config.

- `--memory-limit=<memory-limit-in-megabytes>`

    Overrides the memory limit specified by `MemoryLimit()` in grading config.

- `--no-time-limit`

    Unsets the time limit specified by `TimeLimit()` in grading config.

- `--no-memory-limit`

    Unsets the memory limit specified by `MemoryLimit()` in grading config.

- `--brief`

    Makes the output of the local grading concise by only showing the verdicts.

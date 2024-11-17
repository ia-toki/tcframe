# Problem styles

The `StyleConfig()` method of the problem spec class can be used to specify how the generated test cases will be configured in the grading system.

```cpp
void StyleConfig() {
    // options
}
```

Here are the possible options.

## `NoOutput()`

If called, indicates that the problem does not need the test case output files. No `.out` files will be generated.

## `CustomScorer()`

If called, indicates that the solution's output will be checked with a custom [scorer](../api/helper#scorer) (a.k.a. "checker") program.

Usually, the scorer does not need any test case output, so in most cases the `NoOutput()` option is also called.

For example, a typical problem which has multiple possible outputs, which is to be checked by a custom scorer program, can be configured as follows:

```cpp
void StyleConfig() {
    CustomScorer();
    NoOutput();
}
```

:::info

If the scorer needs test case output files (i.e. the `NoOutput()` option is not called), then the scorer is used during test case generation. It will be used to validate the sample test case outputs which are defined in the problem spec.

In this case, you will need to compile the scorer program prior to running `./runner` to generate the test cases. If the compiled scorer executable is not `./scorer`, you need to pass the command to `./runner` via the `--scorer` option, e.g.

```
./runner --scorer=./my_custom_scorer
```

Scorer will also be used during [local grading](./grading).

:::

## `InteractiveEvaluator()`

If called, indicates that the solution will communicate with a [communicator](../api/helper#communicator) (a.k.a "interactor") program.

This option always requires the `NoOutput()` option to be called.

For example, a typical interactive problem can be configured as follows:

```cpp
void StyleConfig() {
    InteractiveEvaluator();
    NoOutput();
}
```

:::info

Communicator is **not** used during test case generation at all. It will be used only during [local grading](./grading).

:::
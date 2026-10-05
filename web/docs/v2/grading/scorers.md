# Scorers

A scorer decides whether a solution's output is correct for one test case. The default scorer compares the output with the expected `.out` file, token by token.

## The default scorer

When the spec doesn't declare anything, the default scorer does an exact comparison. Whitespace layout is part of the comparison, so compare the output with your expected file if a solution fails on a space or a blank line.

To accept small numeric differences, use the float macros in `StyleConfig`. See [Float tolerance](./float-tolerance).

## A custom scorer

Use a custom scorer when you need a rule the default can't express, for example "accept any answer that is a valid route".

1. Declare it in `StyleConfig`:

   ```cpp
   void StyleConfig() {
       BatchEvaluator();
       CustomScorer();
   }
   ```

2. Put the scorer program in the package folder, as `scorer.cpp` (or as a compiled executable named `scorer`). `make`, `test`, `grade`, and `package` find it and compile it for you:

   ```sh
   tcframe test
   ```

   To use a scorer from somewhere else, pass it explicitly. A `--scorer` option always wins over the package's scorer:

   ```sh
   tcframe test --scorer=../shared/scorer.cpp
   ```

   If the package has both `scorer.cpp` and `scorer`, the source file is used.

## The scorer protocol

The scorer is run once per test case with three arguments:

| Argument | Content |
| --- | --- |
| `argv[1]` | The test case input file |
| `argv[2]` | The expected output file |
| `argv[3]` | The solution's output |

It prints `AC` or `WA` to stdout.

```cpp
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    ifstream tc_out(argv[2]);
    ifstream con_out(argv[3]);

    double expected, got;
    tc_out >> expected;
    con_out >> got;

    cout << (abs(expected - got) < 1e-1 ? "AC" : "WA") << endl;
}
```

## Why `CustomScorer()` is still needed

`CustomScorer()` tells the runner to use a scorer at all. A scorer in the package without this declaration is compiled but not used, so existing 1.x specs keep their behaviour.

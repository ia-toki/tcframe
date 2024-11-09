# Multiple test cases per file

TCFrame supports ICPC-style test cases, where there are multiple test cases in a single file, which preceded by a single line containing the test case count.

The following is how TCFrame will combine multiple test cases into individual files:

- All sample test cases in `SampleTestCaseX()` methods will be combined into `<slug>_sample.{in,out}`.
- All test cases in `TestCases()` method will be combined into `<slug>.{in,out}`.
- All test cases in each of the `TestGroupX()` method will be combined into `<slug>_<X>.{in,out}`.

You need to apply the following changes in order to make a spec file ICPC-style.

## Problem spec class

Add the following as members of `ProblemSpec`:

### Input variable counter

Declare an integer input variable (e.g., `T`) that will hold the number of test cases in a single file.

```cpp
protected:
    int T;

    ...
```

Then, write a method `MultipleTestCasesConfig()`, and call the `Counter()` method with `T` as the argument.

```cpp
void MultipleTestCasesConfig() {
    Counter(T);
}
```

### Output prefix

You can also specify the prefix to be prepended before the output of each test case in a file. For example, in ICPC, usually each test case in the output must be prepended by `Case #X:`, where `X` is the test case number in that file. This can be achieved by calling `OutputPrefix()` method in `MultipleTestCasesConfig()`, with the prefix string as the argument. The string may contain `%d` which will be replaced with the actual test case number. For example:

```cpp
void MultipleTestCasesConfig() {
    Counter(T);
    OutputPrefix("Case #%d: ");
}
```

### Input format

The input format should not contain `T`. It should specify the format of a single test case only. The number of test cases will be automatically prepended in the final combined test case input file.

### Constraints

Constraints can be imposed on `T`, by writing the `MultipleTestCasesConstraints()` method and defining its constraints there.

```cpp
void MultipleTestCasesConstraints() {
    CONS(1 <= T && T <= 20);
}
```

## Test spec class

No changes are necessary. You also don't need to prepend the output prefix in sample test case outputs. Write the sample test cases as if they are not combined into a single file.

## Solution program

Although the input/output format only specifies a single test case, you must write the solution as if the test cases are already combined. In other words, the solution will read the number of test cases in the first line, then that many of test cases, and must output the answer of each test case.

## Example

Here is our sample `sum` problem in ICPC-style.

```cpp
#include <tcframe/spec.hpp>
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    int T;
    int A, B;
    int sum;

    void InputFormat() {
        LINE(A, B);
    }

    void OutputFormat() {
        LINE(sum);
    }

    void GradingConfig() {
        TimeLimit(1);
        MemoryLimit(64);
    }

    void MultipleTestCasesConfig() {
        Counter(T);
        OutputPrefix("Case #%d: ");
    }

    void MultipleTestCasesConstraints() {
        CONS(1 <= T && T <= 20);
    }

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Input({
            "4 6"
        });
        Output({
            "10"
        });
    }

    void TestCases() {
        CASE(A = 1, B = 1);
        CASE(A = 1000, B = 1000);
        CASE(A = 42, B = 100);
        CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
    }
};
```
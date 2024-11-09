# Test spec

`TestSpec` must inherit `tcframe::BaseTestSpec<ProblemSpec>`:

```cpp
class TestSpec : public BaseTestSpec<ProblemSpec> {};
```

Except for private helper functions, every member of `TestSpec` listed below must be `protected`.

## Sample test cases

### `SampleTestCaseX()`

```cpp
virtual void SampleTestCase1() {}
virtual void SampleTestCase2() {}
// ...
virtual void SampleTestCase25() {}
```

Defines the sample test cases (up to 25). The following methods are exposed:

#### `Subtasks()`
```cpp
void Subtasks(std::set<int> subtaskNumbers);
```

Assigns the current sample test case to a set of subtasks, if the problem has [subtasks](../topic-guides/subtasks). If used, this should be the first call in a sample test case.

#### `Input()`

```cpp
void Input(std::vector<std::string> lines);
```

Defines the input as exact literal string, given as list of lines.

#### `Output()`

```cpp
void Output(std::vector<std::string> lines);
```

Defines the input as exact literal string, given as list of lines. It is optional; if not specified, the solution will be run against the sample input to produce the corresponding sample output.

---

Example:

```cpp
void SampleTestCase1() {
    Input({
        "4 6",
        "a b c"
    });
    Output({
        "10"
    });
}
```

## Test cases and test groups

### `TestCases()`

```cpp
virtual void TestCases() {}
```

Defines the test cases.

### `TestGroupX()`
```cpp
virtual void TestGroup1() {}
virtual void TestGroup2() {}
// ...
virtual void TestGroup25() {}
```

Defines the test cases on each test group (up to 25).

### Definitions

The following macro is exposed to define test cases:

#### `CASE()`

```cpp
#define CASE(statement, ...)  /* see below */
```

Defines a test case.

Each **statement** should be one of:

- assignment to an input variables
- private method call that assigns values to one or more input variables

Example:

```cpp
void TestCases() {
    CASE(N = 42, M = 100, randomArray());
    CASE(N = 1000, M = 1000, randomArray());
    CASE(randomEqualNandM(), randomArray());
}
```

## Test case lifecycle

### `BeforeTestCase()`, `AfterTestCase()`

```cpp
virtual void BeforeTestCase() {}
virtual void AfterTestCase() {}
```

Hook up additional logic to run during a [test case lifecycle](../topic-guides/test-cases#test-case-lifecycle).

For each test case, the following things will happen in order:

1. `BeforeTestCase()` is executed.
1. The assignments/method calls inside `CASE()` are executed.
1. `AfterTestCase()` is executed.
1. Input variable values are printed according to the input format.

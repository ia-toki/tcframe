---
toc_max_heading_level: 4
---

# Problem spec

`ProblemSpec` must inherit `tcframe::BaseProblemSpec`:

```cpp
class ProblemSpec : public BaseProblemSpec {};
```

Except for private helper functions, every member of `ProblemSpec` listed below must be `protected`.

---

## Input/output variables

Defined as instance member variables of `ProblemSpec`, which will be referenced in other methods of `ProblemSpec` and `TestSpec`.

There are three supported types of variables:

### Scalar

Variables of built-in integral types (`int`, `long long`, `char`, etc.), built-in floating-point types (`float`, `double`), and `std::string`.

### Vector

`std::vector<T>`, where `T` is a scalar type as defined above. Arrays (`T[]`) are not supported.

### Matrix

`std::vector<std::vector<T>>`, where `T` is a scalar type as defined above. 2D arrays (`T[][]`) are not supported.

**Example**

```cpp
class ProblemSpec : public BaseProblemSpec {
protected:
    int A, B;
    vector<int> parent;
    vector<vector<int>> values;
};
```

---

## Input/output formats

### `InputFormat()`

```cpp
virtual void InputFormat() = 0;
```

Defines the input format. It is mandatory.

---

### `OutputFormat()`, `OutputFormatX()`

```cpp
virtual void OutputFormat() {}

virtual void OutputFormat1() {}
virtual void OutputFormat2() {}
// ...
virtual void OutputFormat5() {}
```

Defines the possible output format(s). If there is only one output format, only `OutputFormat()` can be specified, otherwise multiple `OutputFormatX()` should be specified.

---

### Definitions

The following macros are exposed to define input/output formats:

#### `EMPTY_LINE()`

```cpp
#define EMPTY_LINE()  /* see below */
```

Defines an empty line.

---

#### `RAW_LINE()`

```cpp
#define RAW_LINE(var)  /* see below */
```

Defines a line of raw string (which may contain whitespaces). The variable must be a `std::string`.

**Example**

<table>
<thead>
<tr>
<th>Usage</th>
<th>Variable</th>
<th>Format</th>
</tr>
</thead>
<tbody>
<tr>
<td>

```cpp
void InputFormat() {
    RAW_LINE(S);
}
```
</td>
<td>

**S** = "Hello, world!"
</td>
<td>

```
Hello, world!
```
</td>
</tr>
</tbody>
</table>

---

#### `RAW_LINES()`

```cpp
#define RAW_LINES(var)  /* see below */
```

Defines multiple lines, each consisting of raw string.

Usage:

- `RAW_LINES(<var>);`
- `RAW_LINES(<var>) % SIZE(<number of elements>);`

The variable must be a vector of `std::string`. If the size is not given, then this must be the last segment in the I/O format.

**Example**

<table>
<thead>
<tr>
<th>Usage</th>
<th>Variables</th>
<th>Format</th>
</tr>
</thead>
<tbody>
<tr>
<td>


```cpp
void InputFormat() {
    RAW_LINES(X) % SIZE(2);
    RAW_LINES(Y);
}
```
</td>
<td>

- **X** = \{
  - "Hello, world!",
  - "Happy new year."}
- **Y** = \{
  - "lorem",
  - "ipsum",
  - "dolor sit amet"}
</td>
<td>

```
Hello, world!
Happy new year.
lorem
ipsum
dolor sit amet
```
</td>
</tr>
</tbody>
</table>

---

#### `LINE()`

```cpp
#define LINE(arg, ...)  /* see below */
```

Defines a single line containing space-separated scalar or vector variables. In case of vector variables, the elements are separated by spaces as well.

Usage:

- `LINE(<arg1>, <arg2>, ...);`

where each **arg** is one of:

- `<scalar variable name>`.
- `<vector variable name> % SIZE(<number of elements>)`. The number of elements can be a constant or a scalar variable.
- `<vector variable name>`. Here, the number of elements is unspecified. This kind of element must occur last in a line segment, if any. Elements will be considered until new line is found.

**Example**

<table>
<thead>
<tr>
<th>Usage</th>
<th>Variables</th>
<th>Format</th>
</tr>
</thead>
<tbody>
<tr>
<td>

```cpp
void InputFormat() {
    LINE(N);
    LINE(A % SIZE(3), B);
    LINE(M, C % SIZE(M));
}
```
</td>
<td>

- **N** = 2
- **A** = \{1, 2, 3}
- **B** = \{100, 200, 300, 400}
- **M** = 2
- **C** = \{7, 8}
</td>
<td>

```
2
1 2 3 100 200 300 400
2 7 8
```
</td>
</tr>
</tbody>
</table>

---

#### `LINES()`

```cpp
#define LINES(var, ...)  /* see below */
```

Defines multiple lines, each consisting of space-separated elements of given vector/matrix variables.

Usage:

- `LINES(<var1>, <var2>, ...);`
- `LINES(<var1>, <var2>, ...) % SIZE(<number of elements>);`

where each variable is a vector or matrix.

If the size is not given, this must be the last segment in the I/O format.

If a matrix variable is given, it must occur as the last argument, and the number of rows must match with the number of elements of the other vector variables (if any). It is not required that each row of the matrix consists of the same number of columns.

**Examples**

<table>
<thead>
<tr>
<th>Usage</th>
<th>Variables</th>
<th>Format</th>
</tr>
</thead>
<tbody>
<tr>
<td>

```cpp
void InputFormat() {
    LINES(V) % SIZE(2);
    LINES(X, Y) % SIZE(N);
    LINES(Z);
}
```
</td>
<td>

- **V** = \{1, 2}
- **X** = \{100, 110, 120}
- **Y** = \{200, 210, 220}
- **N** = 3
- **Z** = \{1, 2, 3, 4}

</td>
<td>

```
1
2
100 200
110 210
120 220
1
2
3
4
```
</td>
</tr>

<tr style={{backgroundColor: "inherit"}}>
<td>

```cpp
void InputFormat() {
    LINES(op, data) % SIZE(2);
}
```
</td>
<td>

- **op** = \{"UPDATE", "QUERY"}
- **data** = \{ \{3, 5}, \{7} }
</td>
<td>

```
UPDATE 3 5
QUERY 7
```
</td>
</tr>
</tbody>
</table>

---

#### `GRID()`

```cpp
#define GRID(var)  /* see below */
```

Defines a grid consisting elements of a given matrix variable. If the given matrix variable is of type `char`, the elements in each row is not space-separated, otherwise they are space-separated.

Usage:

- `GRID(<var>) % SIZE(<number of rows>, <number of columns>);`

**Example**

<table>
<thead>
<tr>
<th>Usage</th>
<th>Variables</th>
<th>Format</th>
</tr>
</thead>
<tbody>
<tr>
<td>

```cpp
void InputFormat() {
    GRID(G) % SIZE(2, 2);
    GRID(H) % SIZE(R, C);
}
```
</td>
<td>

- **R** = 2
- **C** = 3
- **G** = \{ \{'a', 'b'}, \{'c', 'd'} }
- **H** = \{ \{1, 2, 3}, \{4, 5, 6} }
</td>
<td>

```
ab
cd
1 2 3
4 5 6
```
</td>
</tr>
</tbody>
</table>

---

### `BeforeOutputFormat()`

```cpp
virtual void BeforeOutputFormat() {}
```

Executed right before the produced output is validated against the output format(s). See [BeforeOutputFormat()](../topic-guides/io-formats#beforeoutputformat) for more details.

---

## Problem style config

### `StyleConfig()`

```cpp
virtual void StyleConfig() {}
```

Defines the options to enable for problem styles. The following methods are exposed:

---

#### `BatchEvaluator()`

```cpp
void BatchEvaluator();
```

Declares that the problem uses batch evaluator.

#### `InteractiveEvaluator()`

```cpp
void InteractiveEvaluator();
```

Declares that the problem uses interactive evaluator.

#### `CustomScorer()`

```cpp
void CustomScorer();
```

Declares that the problem needs a custom scorer.

#### `NoOutput()`

```cpp
void NoOutput();
```

Declares that the problem does not need test case output files.

See [Problem styles](../topic-guides/styles) for more details.

**Example**

```cpp
void StyleConfig() {
    CustomScorer();
    NoOutput();
}
```

---

## Constraints and subtasks

### `MultipleTestCasesConstraints()`

```cpp
virtual void MultipleTestCasesConstraints() {}
```

Defines the constraints to be imposed to the multiple test cases counter.

---

### `Constraints()`

```cpp
virtual void Constraints() {}
```

Defines the constraints to be imposed to the [input/output variables](#inputoutput-variables).

---

### `SubtaskX()`

```cpp
virtual void Subtask1() {}
virtual void Subtask2() {}
// ...
virtual void Subtask25() {}
```

Defines the constraints to be imposed to the input/output variables for each subtask (up to 25).

---

### Definitions

#### `CONS()`

```cpp
#define CONS(predicate)  /* see below */
```

Defines a constraint. **predicate** is a boolean expression, whose value must be completely determined by the values of the input variables (only).

#### `Points()`

```cpp
void Points(double points);
```

Sets the points assigned to a subtask. If not specified, the default is 0. Only available in `SubtaskX()`s.

**Examples**

```cpp
void MultipleTestCasesConstraints() {
    CONS(1 <= T && T <= 20);
}
```

```cpp
void Constraints() {
    CONS(A <= B && B <= 1000);
    CONS(graphDoesNotHaveCycles());
}
```

```cpp
void Subtask1() {
    Points(70);

    CONS(A <= B && B <= 1000);
    CONS(graphDoesNotHaveCycles());
}
```

---

## Multiple test cases config

### `MultipleTestCasesConfig()`

```cpp
virtual void MultipleTestCasesConfig() {}
```

Defines the config for [multiple test cases per file problems](../topic-guides/multi-case).  The following methods are exposed:

#### `Counter()`

```cpp
void Counter(int& var);
```

Sets the input variable that will hold the number of test cases in a file.

#### `OutputPrefix()`

```cpp
void OutputPrefix(std::string prefix);
```

Sets the prefix to be prepended to the output of each test case. It can include ``%d``, which will be replaced by the actual test case number (1-based).

**Example**

``` cpp
void MultipleTestCasesConfig() {
    Counter(T);
    OutputPrefix("Case #%d: ");
}
```

---

## Grading config

### `GradingConfig()`

```cpp
virtual void GradingConfig() {}
```

Defines the config for [local grading](../topic-guides/grading). The following methods are exposed:

---

#### `TimeLimit()`

```cpp
void TimeLimit(int timeLimitInSeconds);
```

Sets the time limit in seconds. If not specified, the default value is 2 seconds.

#### `MemoryLimit()`

```cpp
void MemoryLimit(int memoryLimitInMegabytes);
```

Sets the memory limit in MB. If not specified, the default value is 64 MB.

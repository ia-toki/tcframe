<div align="center">
  <img src="https://raw.githubusercontent.com/ia-toki/tcframe/master/web/static/img/logo.png" height="65" />

  <h1>TCFrame</h1>

  A test case generation framework for competitive programming problems.
 
  <img alt="CI" src="https://github.com/ia-toki/tcframe/workflows/ci/badge.svg"/>
  <a href="https://github.com/ia-toki/tcframe/blob/master/LICENSE.txt"><img alt="License" src="https://img.shields.io/github/license/ia-toki/tcframe.svg"/></a>
</div>

<br>

TCFrame is a C++ framework for generating test cases of competitive programming problems. This framework helps problem writers prepare test cases in a structured manner and ensures that the generated test cases are valid according to the specified constraints.

Consult the complete documentation at https://tcframe.toki.id.

Example high-level usage:

1. Specify input/output variables.

   ```cpp
   int A, B;
   int sum;
   ```

1. Specify input/output formats, using a rich set of format macros.

   ```cpp
   void InputFormat() {
       LINE(A, B); // A line containing space-separated A and B
   }
   void OutputFormat() {
       LINE(sum);
   }
   ```

1. Specify the grading configuration.

   ```cpp
   void GradingConfig() {
       TimeLimit(1);
       MemoryLimit(64);
   }
   ```

1. Specify the constraints. Subtasks are supported.

   ```cpp
   void Constraints() {
       CONS(1 <= A && A <= 1000);
       CONS(1 <= B && B <= 1000);
   }
   ```

1. Specify the sample test cases.

   ```cpp
   void SampleTestCase1() {
       Input({
           "2 8"
       });
       Output({
           "10"
       });
   }
   void SampleTestCase2() {
       Input({
           "42 100"
       });
       Output({
           "142"
       });
   }
   ```

1. Specify the official test cases. Simple random number generator is available.

   ```cpp
   void TestCases() {
       CASE(A = 1, B = 1);
       CASE(A = 77, B = 99);
       CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
   }
   ```

1. Write and compile the official solution to this problem, using any programming language you wish. Of course, it is the infamous A+B problem.

   ```cpp
   #include <iostream>
   using namespace std;

   int main() {
       int A, B;
       cin >> A >> B;
       cout << (A + B) << endl;
   }
   ```

1. Run the generator. Actual test cases (`.in` and `.out` files) will be generated. Profit!

1. If you ever specified an invalid test case, such as `CASE(A = 0, B = 1)`, you will get a nice error message:

   ```
     sum_4: FAILED
       Description: A = 0, B = 1
       Reasons:
       * Does not satisfy constraints, on:
         - 1 <= A && A <= 1000
   ```

## tcframe 2.0 (v2)

tcframe 2.0 keeps the C++ `spec.cpp` format and adds a Python CLI (`scripts/tcframe`, source in `cli/`). The CLI compiles the spec, the solutions, and any helpers automatically, checks solutions against their expected verdicts, and packages the problem for online judges. Requirements: Python >= 3.10 and GCC with C++17.

Set up once (same as 1.x, see [Installation](https://tcframe.toki.id/installation)):

```sh
export TCFRAME_HOME=~/tcframe   # this checkout
alias tcframe=$TCFRAME_HOME/scripts/tcframe
```

Workflow, inside an empty problem directory:

```sh
tcframe new --template=batch                          # writes ./spec.cpp
mkdir -p solutions/ref                                # put the reference solution here
tcframe make --solution=solutions/ref/solution.cpp    # build/spec.yml + build/tc/
tcframe test                                          # grade solutions/ against expected verdicts
tcframe package --solution=solutions/ref/solution.cpp # build/<slug>-source.zip, build/<slug>.zip
```

Source package layout:

```
mypkg/
  spec.cpp
  metadata.yml            optional, passed through to judges
  solutions/
    ref/solution.cpp      exactly one reference solution (required)
    ac/ wa/ tle/ rte/     expected verdict for every file inside
    ok-75/                expected partial verdict with score 75
    failed/               expected WA, TLE, or RTE
  scorer.cpp | scorer/    optional custom scorer
  languages/*.yml         optional language definitions
```

`tcframe test` exits non-zero when any solution does not match its directory. The distribution package (`build/<slug>.zip`) contains `spec.yml`, `tc/`, and a `validator` program.

Available templates for `tcframe new`: `batch`, `batch-subtasks`, `interactive`, `interactive-subtasks`. Examples of complete v2 packages are in [`examples/`](examples/).

## Features

TCFrame supports:

- Batch and interactive problems.
- ICPC-style problems and IOI-style problems with subtasks and points.
- Multiple test cases per file.
- Local grading against the generated test cases, with time and memory limits.
- Simple random number generation helper.

## Requirements

TCFrame requires:

- Linux/macOS. Windows is not supported.
- GCC which supports C++17.

## Motivations

**Why do we need test case generators?**

- Writing test cases manually is error-prone and time-consuming.
- To enable distributing the test cases as a single, small generator file. No need to send 20 MB of `testcases.zip` over email anymore.
- During problem development, constraints often change. Using a generator, we can easily amend the constraints and rerun the generator when needed.

**Why do we need a framework for that?**

- Not everyone knows how to write a good test cases generator.
- To avoid writing repetitive and boring tasks. For example: creating test case files with correct suffixes (`foo_1.in`, `foo_1.out`), running the official solution against the test case input files, etc.
- To have a consistent format for generators, so that problem writers in a contest can better collaborate in writing test case generators.

## Credits

TCFrame is based on a paper submitted to IOI conference in 2015: [Introducing tcframe: A Simple and Robust Test Cases Generation Framework](https://ioinformatics.org/journal/v9_2015_57_73.pdf), written by **Ashar Fuadi**.

TCFrame was mainly inspired from [testlib](https://github.com/MikeMirzayanov/testlib), written by Mike Mirzayanov et al.

## License

TCFrame is released under MIT license.

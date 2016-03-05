# tcframe 0.6.0

[![Build Status](https://travis-ci.org/fushar/tcframe.svg?branch=master)](https://travis-ci.org/fushar/tcframe)
[![Coverage Status](http://codecov.io/github/fushar/tcframe/coverage.svg?branch=master)](http://codecov.io/github/fushar/tcframe?branch=master)
[![Documentation Status](https://readthedocs.org/projects/tcframe/badge/?version=latest)](https://readthedocs.org/projects/tcframe)

**tcframe** is a C++ framework for generating test cases of competitive programming problems. This framework helps problem writers prepare test cases in a structured manner and ensures that the generated test cases are valid according to the specified constraints.

Consult the complete documentation at https://tcframe.readthedocs.org/en/latest/.

At a very high level, with some details omitted, it works as follows:

1. You specify input/output variables.

    ```cpp
    int A, B;
    int sum;
    ```

1. You specify input/output formats.

    ```cpp
    void InputFormat() {
        LINE(A, B); // A line containing space-separated A and B
    }
    void OutputFormat() {
        LINE(sum);
    }
    ```

1. You specify the constraints.

    ```cpp
    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }
    ```

1. You specify the sample test cases.

    ```cpp
    void SampleTestCases() {
        SAMPLE_CASE({
            "2 8"
        });
        SAMPLE_CASE({
            "42 100"
        });
    }
    ```

1. You specify the official test cases. Random number generator is available.

    ```cpp
    void TestCases() {
        CASE(A = 1, B = 1);
        CASE(A = 77, B = 99);
        CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
    }
    ```

1. You write the official solution to this problem, using any programming language you wish. Oh, haven't we told you what the actual problem we have been talking about is? Yes, it is the infamous A+B problem!

    ```cpp
    #include <iostream>
    using namespace std;

    int main() {
        cin >> A >> B;
        cout << (A + B) << endl;
    }
    ```

1. You run the generator. Actual test cases (.in and .out files) will be generated. Profit!

1. If you ever specified an invalid test case, such as `CASE(A = 0, B = 1)`, you will get a nice error message:

	```
      aplusb_4: FAILED
        Description: A = 0, B = 1
        Reasons:
        * Does not satisfy constraints, on:
          - 1 <= A && A <= 1000
	```

## Features

As of the current version, **tcframe** supports:

- Standard batch problems; i.e., problems which requires the solution to read from stdin and print to stdout.
- Constraints specified in IOI-style subtasks.
- Multiple test cases per file.
- Simulating submission against the generated test cases.
- Specifying time and memory limits.
- Basic random number generation helper.

## Requirements

**tcframe** requires:

- Linux/OS X. Windows is currently not supported yet
- GCC ≥ 4.7. **tcframe** relies heavily on C++11 features

## Frequently Asked Questions

**Why do we even need to write a generator for test cases, in the first place?**

- That's primarily because writing test cases manually is error-prone and time-consuming.
- To enable distributing the test cases as a single, small generator file. No need to send 20 MB testcases.zip over email anymore.
- During problem development, constraints often change. Using a generator, we can easily fix the constraint and just run the generator again.

**OK. But why do we need a framework for that?**

- The main problem is that not all people know how to write a good test cases generator.
- To avoid writing repetitive and boring tasks. For example: creating test case files with correct suffixes (foo_1.in, foo_1.out), running official solution against the test case input files, etc.
- To make all problems in a contest have test cases generator with consistent format.

## Credits

**tcframe** is being heavily developed by **Ashar Fuadi**. It is based on a paper submitted to IOI conference in 2015: [Introducing tcframe: A Simple and Robust Test Cases Generation Framework](http://ioinformatics.org/oi/files/volume9.pdf#page=59), written by the same author.

**tcframe** was mainly inspired from [testlib](https://github.com/MikeMirzayanov/testlib), written by Mike Mirzayanov et al.

## License

**tcframe** is released under MIT license.

Issues and pull requests are welcome.

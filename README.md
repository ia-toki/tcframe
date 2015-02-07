# tcframe

[![Build Status](https://travis-ci.org/tcframe/tcframe.svg?branch=master)](https://travis-ci.org/tcframe/tcframe)

**tcframe** is a framework for generating test cases, designed especially for problems in competitive programming contests.

Currently it is in very early stage of development. Here is an example of the current planned syntax of a generator written using tcframe:

```
#include "tcframe/tcframe.h"

using namespace tcframe;

struct Problem : BaseProblem {
    int A, B;

    void Config() {
        setSlug("example");
    }

    void InputFormat() {
        line() % A, B;
    }

    void Subtask1() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 2000000000);
        CONS(1 <= B && B <= 2000000000);
    }
};

struct Generator : BaseGenerator<Problem> {
    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 2);
        CASE(A = 100, B = 200);
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 1001, B = 10000);
        CASE(A = 2000000000, B = 2000000000);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
```

The above program will generate valid test cases.

Here is an example of a program that generate some invalid test cases:

```
#include "tcframe/tcframe.h"

using namespace tcframe;

struct Problem : BaseProblem {
    int A, B;

    void Config() {
        setSlug("example");
    }

    void InputFormat() {
        line() % A, B;
    }

    void Subtask1() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 2000000000);
        CONS(1 <= B && B <= 2000000000);
    }
};

struct Generator : BaseGenerator<Problem> {
    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 2000);
        CASE(A = 100, B = 200);
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 1001, B = 10000);
        CASE(A = 1, B = 2);
        CASE(A = 2000000000, B = 2000000000);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
```

The above program will output:

```
Generating test cases...

[ SAMPLE TEST CASES ]

[ TEST GROUP 1 ]
  example_1_1: FAILED
    Description: (A = 1, B = 2000)
    Reasons:
    * Does not satisfy subtask 1, on constraints:
      - 1 <= B && B <= 1000

  example_1_2: OK

[ TEST GROUP 2 ]
  example_2_1: OK
  example_2_2: FAILED
    Description: (A = 1, B = 2)
    Reasons:
    * Satisfies subtask 1 but is not assigned to it

  example_2_3: OK
```

The API documentation will be available soon, stay tuned!

# tcframe

[![Build Status](https://travis-ci.org/ia-toki/tcframe.svg?branch=master)](https://travis-ci.org/ia-toki/tcframe)
[![Coverage Status](https://coveralls.io/repos/ia-toki/tcframe/badge.svg?branch=master)](https://coveralls.io/r/ia-toki/tcframe?branch=master)

**tcframe** is a framework for generating test cases, designed especially for problems in competitive programming contests.

The current latest version is **0.1**.

This project is under heavy development. No sufficient documentations are available at the moment. However, here are some previews of generator programs that should be self-explanatory.

### Generating test cases for problems without subtasks

```
#include "tcframe/tcframe.hpp"

using namespace tcframe;

class Problem : public BaseProblem {
protected:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        LINE(A, B);
        LINE(K);
    }

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
        CONS(0 <= K && K <= 100);
    }
};

class Generator : public BaseGenerator<Problem> {
protected:
    void Config() {
        setBaseDir("tc");
        setSolution("./solution");
    }

    void TestCases() {
        CASE(A = 1, B = 100, K = 7);
        CASE(A = 100, B = 2000, K = 0);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
```

The above program, when run, will output:

```
Generating test cases...

[ SAMPLE TEST CASES ]

[ OFFICIAL TEST CASES ]
  problem_1: OK
  problem_2: FAILED
    Description: (A = 100, B = 2000, K = 0)
    Reasons:
    * Does not satisfy constraints, on:
      - 1 <= B && B <= 1000
```

### Generating test cases for problems with subtasks

```
#include "tcframe/tcframe.hpp"

using namespace tcframe;

class Problem : public BaseProblem {
protected:
    int A, B;
    int K;

    void Config() {
        setSlug("problem");
    }

    void InputFormat() {
        LINE(A, B);
        LINE(K);
    }

    void Subtask1() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
        CONS(0 <= K && K <= 100);
    }

    void Subtask2() {
        CONS(1 <= A && A <= 2000000000);
        CONS(1 <= B && B <= 2000000000);
        CONS(0 <= K && K <= 10000);
    }

    void Subtask3() {
        CONS(A == 0);
        CONS(B == 0);
        CONS(0 <= K && K <= 100);
    }
};

class Generator : public BaseGenerator<Problem> {
protected:
    void Config() {
        setBaseDir("tc");
        setSolution("./solution");
    }

    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(A = 1, B = 100, K = 7);
        CASE(A = 100, B = 2000, K = 0);
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(A = 1, B = 2, K = 1);
        CASE(A = 0, B = 0, K = 100);
    }
};

int main() {
    Generator gen;
    gen.generate();
}
```

The above program, when run, will output:

```
Generating test cases...

[ SAMPLE TEST CASES ]

[ TEST GROUP 1 ]
  problem_1_1: OK
  problem_1_2: FAILED
    Description: (A = 100, B = 2000, K = 0)
    Reasons:
    * Does not satisfy subtask 1, on constraints:
      - 1 <= B && B <= 1000

[ TEST GROUP 2 ]
  problem_2_1: FAILED
    Description: (A = 1, B = 2, K = 1)
    Reasons:
    * Satisfies subtask 1 but is not assigned to it
  problem_2_2: FAILED
    Description: (A = 0, B = 0, K = 100)
    Reasons:
    * Does not satisfy subtask 2, on constraints:
      - 1 <= A && A <= 2000000000
      - 1 <= B && B <= 2000000000
    * Satisfies subtask 3 but is not assigned to it
```

### Maintainers

**tcframe** is being developed and maintained by **Ashar Fuadi**. Contact me at fushar [at] gmail [dot] com if you have any question.

# tcframe 0.5.0

[![Build Status](https://travis-ci.org/fushar/tcframe.svg?branch=master)](https://travis-ci.org/fushar/tcframe)
[![Coverage Status](http://codecov.io/github/fushar/tcframe/coverage.svg?branch=master)](http://codecov.io/github/fushar/tcframe?branch=master)
[![Documentation Status](https://readthedocs.org/projects/tcframe/badge/?version=latest)](https://readthedocs.org/projects/tcframe)

**tcframe** is a framework for generating test cases, designed especially for problems in competitive programming contests.

Consult the documentation at https://tcframe.readthedocs.org/en/latest/.

### Example Code

Below is an example of a program written using tcframe. This program (called runner program) generates test cases when executed.

```c++
#include "tcframe/runner.hpp"
using namespace tcframe;

#include <vector>
using namespace std;

class Problem : public BaseProblem {
protected:
    int N, K, result;
    vector<int> A;

    void InputFormat() {
        LINE(N, K);
        LINE(A % SIZE(N));
    }

    void OutputFormat() {
        LINE(result);
    }

    void Subtask1() {
        CONS(1 <= N && N <= 10);
        CONS(1 <= K && K <= 10);
        CONS(eachElementBetween(A, -100000, 100000));
    }

    void Subtask2() {
        CONS(1 <= N && N <= 100);
        CONS(1 <= K && K <= 100);
        CONS(eachElementBetween(A, -100000, 100000));
    }

private:
    bool eachElementBetween(const vector<int>& A, int lo, int hi) {
        for (int x : A) {
            if (x < lo || x > hi) {
                return false;
            }
        }
        return true;
    }
};

class Generator : public BaseGenerator<Problem> {
protected:
    void TestGroup1() {
        assignToSubtasks({1, 2});

        CASE(N = 1, K = 2, randomArray());
        CASE(N = 10, K = 10, randomArray());
    }

    void TestGroup2() {
        assignToSubtasks({2});

        CASE(N = 11, K = 10, randomArray());
        CASE(N = 100, K = 100, randomArray());
    }

private:
    void randomArray() {
        A.clear();
        for (int i = 0; i < N; i++) {
            A.push_back(rnd.nextInt(-100000, 100000));
        }
    }
};

int main(int argc, char* argv[]) {
    Runner<Problem> runner(argc, argv);

    runner.setGenerator(new Generator());
    return runner.run();
}
```

### Usage

There have been several competitions that used **tcframe** as the test cases generation framework for the problems:

- Asia-Pacific Informatics Olympiad (APIO) 2015
- Indonesia National Olympiad in Informatics 2015
- Indonesia IOI training camps in 2015

### Credits

**tcframe** is created by **Ashar Fuadi**. Contact me at fushar [at] gmail [dot] com if you have any question.

### License

**tcframe** is released under MIT license.

Introduction
============

**tcframe** is a framework for generating test cases, designed especially for problems in competitive programming
contests. If you are a problem author and want to create test cases for your problem, you are on the right site!

Philosophy
----------

First, let's begin with a philosophy on why this project is useful.

**Why do we need to write generator for test cases?**

- Writing test cases manually is error-prone and time-consuming.
- We can easily distribute the test cases, as a single, small file.
- We can easily modify test case values in the future, for example when there are changes in the constraints.

**Why do we need a test cases generation framework?**

- Not all people know how to write a test cases generator.
- To avoid writing repetitive and boring tasks, for example, creating test case files with appropriate numbering, running official solution against the test case input files, etc.
- To make all problems in a contest have test cases generator with consistent format.

For the above reasons, **tcframe** was developed.

Features
----------------

As a framework, **tcframe** offers the following exciting features:

- Excluding the official solution, we only need to write a single C++ program, called runner program.
- The runner program has a syntax that is easy to read.
- The runner program is nicely structured in several parts, similar to the problem statement: input format, output format, constraints/subtasks, etc.
- The resulting test cases are validated according to the specification constraints/subtasks.
- All errors (for example: constraints unsatisfiability) are presented to the users in human-readable format so that they can easily fix the errors.

Examples
--------

Here are some example of runner programs written using **tcframe** framework:

For problems without subtasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. sourcecode:: cpp

    #include "tcframe/runner.hpp"
    using namespace tcframe;

    class Problem : public BaseProblem {
    protected:
        int A, B;
        int K;

        int result;

        void InputFormat() {
            LINE(A, B);
            LINE(K);
        }

        void OutputFormat() {
            LINE(result);
        }

        void Constraints() {
            CONS(1 <= A && A <= 1000);
            CONS(1 <= B && B <= 1000);
            CONS(0 <= K && K <= 100);
        }
    };

    class Generator : public BaseGenerator<Problem> {
    protected:
        void SampleTestCases() {
            SAMPLE_CASE({
                "1 1",
                "100"
            });
        }

        void TestCases() {
            CASE(A = 1, B = 100, K = 7);
            CASE(A = 100, B = 2000, K = 0);
        }
    };

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

The above runner program, when run, will output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      problem_sample_1: OK

    [ OFFICIAL TEST CASES ]
      problem_1: OK
      problem_2: FAILED
        Description: A = 100, B = 2000, K = 0
        Reasons:
        * Does not satisfy constraints, on:
          - 1 <= B && B <= 1000

For problems with subtasks
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. sourcecode:: cpp

    #include "tcframe/runner.hpp"
    using namespace tcframe;

    class Problem : public BaseProblem {
    protected:
        int A, B;
        int K;

        int result;

        void InputFormat() {
            LINE(A, B);
            LINE(K);
        }

        void OutputFormat() {
            LINE(result);
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
        void SampleTestCases() {
            SAMPLE_CASE({
                "1  1",
                "100"
            }, {1, 2});
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

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

The above runner program, when run, will output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      problem_sample_1: FAILED
        Reasons:
        * Cannot parse for variable `B`. Found: <whitespace>

    [ TEST GROUP 1 ]
      problem_1_1: OK
      problem_1_2: FAILED
        Description: A = 100, B = 2000, K = 0
        Reasons:
        * Does not satisfy subtask 1, on constraints:
          - 1 <= B && B <= 1000

    [ TEST GROUP 2 ]
      problem_2_1: FAILED
        Description: A = 1, B = 2, K = 1
        Reasons:
        * Satisfies subtask 1 but is not assigned to it
      problem_2_2: FAILED
        Description: A = 0, B = 0, K = 100
        Reasons:
        * Does not satisfy subtask 2, on constraints:
          - 1 <= A && A <= 2000000000
          - 1 <= B && B <= 2000000000
        * Satisfies subtask 3 but is not assigned to it

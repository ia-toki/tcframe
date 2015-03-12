Introduction
============

**tcframe** is a framework for generating test cases, designed especially for problems in competitive programming
contests. Using **tcframe**, we only need to write a single C++ generator program (and of course an official solution).
When run, the generator will produce the test case files.

Examples
--------

Here are some example of generator files written using **tcframe** framework:

For problems without subtasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. sourcecode:: cpp

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

    int main() {
        Generator gen;
        return gen.generate();
    }

The above generator program, when run, will output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      problem_sample_1: OK

    [ OFFICIAL TEST CASES ]
      problem_1: OK
      problem_2: FAILED
        Description: (A = 100, B = 2000, K = 0)
        Reasons:
        * Does not satisfy constraints, on:
          - 1 <= B && B <= 1000

For problems with subtasks
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. sourcecode:: cpp

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

    int main() {
        Generator gen;
        return gen.generate();
    }

The above generator program, when run, will output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      problem_sample_1: FAILED
        Reasons:
        * Cannot parse for variable `B`. Found: <whitespace>

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

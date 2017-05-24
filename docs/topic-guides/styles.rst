.. _styles:

Problem Styles
==============

The ``StyleConfig()`` method of the problem spec class can be used to configure several aspects related to the nature of the problem itself and is independent from the test spec.

.. sourcecode:: cpp

    void StyleConfig() {
        // option specifications
    }

Evaluator
---------

An evaluator specifies how to run a solution against a test case. Two types of evaluators are supported:

Batch
*****

Enabled by calling ``BatchEvaluator()``. This is the default evaluator if none is specified. The solution must read the test cases from the standard input and print the output to the standard output.

The following options are further configurable:

- ``CustomScorer()``

  By default, the output will be checked with the default ``diff`` program, unless a custom **scorer** is specified. See the **Helper programs** section on how to write a scorer.

- ``NoOutput()``

  If the problem is using a custom scorer and it does not depend on test case output of any test case, then this option can be enabled. If enabled, then ``.out`` files will not be generated, and it is not allowed to specify ``Output()`` in sample test cases.

Interactive
***********

Enabled by calling ``InteractiveEvaluator()``. The solution will participate in a 2-way communication with a special program called **communicator**, which will ultimately print the verdict of the solution. See the **Helper programs** section on how to write a communicator.

----

Helper programs
---------------

Scorer
******

A scorer is a program which decides the verdict of a test case. It will receive the following arguments:

- argv[1]: test case input filename
- argv[2]: test case output filename
- argv[3]: contestant's produced output filename

It must print the test case verdict to the standard output, which is a line consisting of either:

- ``AC``: indicates that the contestant's output is correct. It will be given 100 / (number of test cases in its subtask) points.
- ``WA``: indicates that the contestant's output is incorrect. It will be given 0 points.
- ``OK``: indicates that the contestant's output is partially correct. The second line must contain a floating-point number denoting the points. For example:

  ::

      OK
      9


The scorer must be compiled prior to test cases generation/local grading, and the execution command should be passed to the runner program as the ``--scorer`` option. For example:

::

    ./runner grade --solution=./solution_alt --scorer=./my_custom_scorer

The default scorer command is ``./scorer`` if not specified.

Here is an example scorer which gives AC if the contestant's output differs not more than 1e-9 from the official output.

.. sourcecode:: cpp

    #include <bits/stdc++.h>
    using namespace std;

    int wa() {
        cout << "WA" << endl;
        return 0;
    }

    int ac() {
        cout << "AC" << endl;
        return 0;
    }

    int main(int argc, char* argv[]) {
        ifstream tc_in(argv[1]);
        ifstream tc_out(argv[2]);
        ifstream con_out(argv[3]);

        double tc_ans;
        tc_out >> tc_ans;

        double con_ans;
        if (!(con_out >> con_ans)) {
            return wa();
        }

        if (abs(tc_ans - con_ans) < 1e-9) {
            return ac();
        } else {
            return wa();
        }
    }

Communicator
************

A communicator is a program which performs 2-way communication with the solution program, and then decides the verdict. It will receive the following (only) argument:

- argv[1]: test case input filename

During the communication, the communicator can read the solution program's output from the standard input, and can give input to the solution program by writing to the standard output. Make sure the communicator flushes after every time it writes output. Ultimately, the communicator must print the test case verdict to the standard error, with the same format as a scorer as described in the previous section.

The communicator must be compiled prior to local grading, and the execution command should be passed to the runner program as the ``--communicator`` option. For example:

::

    ./runner grade --solution=./solution_alt --communicator=./my_communicator

The default scorer command is ``./communicator`` if not specified.

Here is an example communicator program in a typical binary search problem.

.. sourcecode:: cpp

    #include <bits/stdc++.h>
    using namespace std;

    int ac() {
        cerr << "AC" << endl;
        return 0;
    }

    int wa() {
        cerr << "WA" << endl;
        return 0;
    }

    int main(int argc, char* argv[]) {
        ifstream tc_in(argv[1]);

        int N;
        tc_in >> N;

        int guesses_count = 0;

        while (true) {
            int guess;
            cin >> guess;

            guesses_count++;

            if (guesses_count > 10) {
                return wa();
            } else if (guess < N) {
                cout << "TOO_SMALL" << endl;
            } else if (guess > N) {
                cout << "TOO_LARGE" << endl;
            } else {
                return ac();
            }
        }
    }

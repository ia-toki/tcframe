.. _styles:

Problem Styles
==============

Currently, **tcframe** supports only **batch**-style problems, where the solution is expected to read the test cases from the standard input and write the answers to the standard output. There are some configurable options to this behavior, which can be specified in the ``StyleConfig()`` method of the problem spec class.

.. sourcecode:: cpp

    void StyleConfig() {
        // option specifications
    }

The available options are as follows.

Custom scorer
-------------

Enabled by calling ``CustomScorer()`` inside ``StyleConfig()``.

A scorer is a program which decides the verdict of a test case. By default, the scorer is the simple ``diff`` program. If custom scorer is enabled, then you must provide the custom scorer program.

The custom scorer will receive the following arguments:

- argv[1]: test case input filename
- argv[2]: test case output filename
- argv[3]: contestant's produced output filename

The custom scorer must print the test case verdict to the standard output, which is a line consisting of either:

- ``AC``: indicates that the contestant's output is correct. It will be given 100 / (number of test cases in its subtask) points.
- ``WA``: indicates that the contestant's output is incorrect. It will be given 0 points.
- ``OK``: indicates that the contestant's output is partially correct. The second line must contain a floating-point number denoting the points. For example:

  ::

      OK
      9


The custom scorer must be compiled prior test cases generation/local grading, and the execution command should be passed to the runner program as the ``--scorer`` option. For example:

::

    ./runner grade --solution=./solution_alt --scorer=./my_custom_scorer

The default scorer command is ``./scorer`` if not specified.

Here is an example custom scorer which gives AC if the contestant's output differs not more than 1e-9 with the official output.

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

No output
---------

Enabled by calling ``NoOutput()`` inside ``StyleConfig()``.

Sometimes, a problem does not need test case output files (``.out``) because the scoring is done by a custom score alone. If this option is enabled, then ``.out`` files will not be generated, and it is not allowed to specify ``Output()`` in sample test cases.

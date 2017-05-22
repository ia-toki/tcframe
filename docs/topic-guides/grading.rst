.. _grading:

Local grading
=============

**tcframe** allows you to grade solutions locally, on your machine.

Before grading a solution, you must have already generated the test cases:

::

    ./runner

Then, you can "grade" a solution, by executing:

::

    ./runner grade [--solution=<solution-command>]

where ``<solution-command>`` is the command for executing the solution. If it is omitted, the default is ``./solution``.

For example, suppose you have written a problem package for a problem. Your friend also has written an alternate solution to the problem, and he wants to check whether his solution agrees with yours. Let's assume that his solution file is ``solution_alt.cpp``. Compile it into ``solution_alt``, place it in the problem package, and then run

::

    ./runner grade --solution=./solution_alt

The verdict of each test case will be shown. The verdict will be one of the following:

Accepted (AC)
    The output produced by the solution matches.

Wrong Answer (WA)
    The output produced by the solution does not match. The diff will be shown, truncated to the first 10 lines.

Runtime Error (RTE)
    The solution crashed or used memory above the limit, if specified.

Time Limit Exceeded (TLE)
    The solution did not stop within the time limit, if specified.

Internal Error (ERR)
    Custom scorer (if any) crashed or did not give valid verdict.

The verdict of each subtask will be also shown. The verdict of a subtask is the worst verdict of all verdicts of test cases that are assigned to it. Here, RTE is worse than WA, and WA is worse than AC.

Here is a sample output of a local grading for problems with subtasks.

.. sourcecode:: bash

    Local grading with solution command: './solution_alt'...

    [ SAMPLE TEST CASES ]
      k-product_sample_1: Accepted

    [ TEST GROUP 1 ]
      k-product_1_1: Accepted

    [ TEST GROUP 2 ]
      k-product_2_1: Accepted
      k-product_2_2: Accepted
      k-product_2_3: Accepted

    [ TEST GROUP 3 ]
      k-product_3_1: Accepted
      k-product_3_2: Wrong Answer
        * scorer: Diff:
    (expected) [line 01]    11
    (received) [line 01]    12

      k-product_3_3: Accepted

    [ TEST GROUP 4 ]
      k-product_4_1: Accepted
      k-product_4_2: Accepted
      k-product_4_3: Accepted
      k-product_4_4: Accepted
      k-product_4_5: Accepted
      k-product_4_6: Runtime Error
        * Execution of submission failed:
          - Exit code: 1
          - Standard error:

    [ RESULT ]
      Subtask 1: Accepted
      Subtask 2: Wrong Answer
      Subtask 3: Runtime Error

and here is for problems without subtasks

.. sourcecode:: bash

    Local grading with solution command: './solution_alt'...

    [ SAMPLE TEST CASES ]
      k-product_sample_1: Accepted

    [ OFFICIAL TEST CASES ]
      k-product_1: Accepted
      k-product_2: Accepted
      k-product_3: Accepted
      k-product_4: Wrong Answer
        * scorer Diff:
    (expected) [line 01]    11
    (received) [line 01]    12

    [ RESULT ]
      Wrong Answer

This local grading feature is useful for creating "unit tests" for your test cases. For each problem, you can write many solutions with different intended results. For example, ``solution_123.cpp`` should pass subtasks 1 - 3; ``solution_12.cpp`` should pass subtasks 1 and 2 but not subtask 3, etc.

Notes
-----

Internally, **tcframe** uses ``ulimit`` to limit the time and memory used when running the solution. Unfortunately, there is no easy way to restrict memory limit on OS X, so the memory limit will be always ignored when using this feature on OS X.

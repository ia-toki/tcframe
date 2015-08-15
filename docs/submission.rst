Simulating submission
=====================

**tcframe** allows you to simulate solution submission locally, on your machine.

Before simulating a submission, you must have generated the test cases:

.. sourcecode:: bash

    ./runner

Then, you can simulate a submission, by executing:

.. sourcecode:: bash

    ./runner submit [<submissionCommand>]

where <submissionCommand> is the command for executing the submission. If it is omitted, then the command will be the original solution command used in the generator.

For example, suppose you have written a generator for a problem. Your friend also has written an alternate solution to the problem, and he wants to check whether his solution agrees with yours. Let's assume that his solution file is **alt_solution**.cpp. Compile it into **alt_solution**, place it in the same directory of the runner, and then run

.. sourcecode:: bash

    ./runner submit ./alt_solution

The verdict of each test case will be shown. The verdict will be one of the following:

Accepted (AC)
    The output produced by the submission matches.

Wrong Answer (WA)
    The output produced by the submission does not match. The diff will be shown, truncated to the first 10 lines.

Runtime Error (RTE)
    The submission crashed or used memory above the limit, if specified.

Time Limit Exceeded (TLE)
    The submission did not stop within the time limit, if specified.

The verdict of each subtask will be also shown. The verdict of a subtask is the worst verdict of all verdicts of test cases that are assigned to it. Here, RTE is worse than WA, and WA is worse than AC.

Here is a sample output of a submission simulation for problems with subtasks.

.. sourcecode:: bash

    Submitting...

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
        * Diff:
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

    Submitting...

    [ SAMPLE TEST CASES ]
      k-product_sample_1: Accepted

    [ OFFICIAL TEST CASES ]
      k-product_1: Accepted
      k-product_2: Accepted
      k-product_3: Accepted
      k-product_4: Wrong Answer
        * Diff:
    (expected) [line 01]    11
    (received) [line 01]    12

    [ RESULT ]
      Wrong Answer

This submission simulation feature is useful for creating "unit tests" for your test cases. For each problem, you can write many solutions with different intended results. For example, solution_123.cpp should pass subtasks 1 - 3; solution_12.cpp should pass subtasks 1 and 2 but not subtask 3, etc.

You can also specify options. See :ref:`Command-line options <api-ref-command-line-options>` for available command-line options. The most useful optionr are specifying time and memory limit.

Brief output
------------

If you want to automate checking the result of each solution, you can set the output of the submission to be "brief", i.e., concise and easy to parse by another program. Just pass the command-line option **-**\ **-**\ **brief**:

.. sourcecode:: bash

    ./runner submit ./alt_solution --brief

Here is a sample brief output for problems with subtasks:

.. sourcecode:: bash

    1:AC
    2:WA
    3:RTE

And here is for problems without subtasks:

.. sourcecode:: bash

    WA

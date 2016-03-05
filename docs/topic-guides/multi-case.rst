.. _multi-case:

Multiple test cases per file
============================

**tcframe** supports ICPC-style problems as well. In this style, for each of the :code:`SampleTestCases()`, :code:`TestCases()`, and :code:`TestGroupX()` methods, the test cases will be combined into a single file. The file is prepended with a single line containing the number of test cases in it.

To write an ICPC-style test cases generator, first write the runner as usual, assuming that the problem not ICPC-style. Then, apply the following changes.

----

Problem specification class
---------------------------

- Declare an integer input variable (e.g., :code:`T`) that will hold the number of test cases in a single file.

  .. sourcecode:: cpp

      protected:
          int T;

          ...

- In the problem configuration, call the :code:`setMultipleTestCasesCount()` method with the previous variable as the argument.

  .. sourcecode:: cpp

      void Config() {
          ...

          setMultipleTestCasesCount(T);

          ...
      }

- The input format specification should not contain :code:`T`. It should specify the format of a single test case only. The number of test cases will be automatically prepended in the final combined test case input file.

- Constraints can be imposed on :code:`T`, by writing the method :code:`MultipleTestCasesConstraints()` method and defining constraints there.

  .. sourcecode:: cpp

      void MultipleTestCasesConstraints() {
          CONS(1 <= T && T <= 20);
      }

----

Generation specification class
------------------------------

No changes are necessary.

----

Solution program
----------------

Although the input format only specifies a single test case, the solution should read the number of test cases in the first line. In other words, the solution will read the final combined test cases input file.

----

Notes
-----

Currently, it is impossible to prepend each output with something like :code:`Case #X`, where :code:`X` is the test case number.
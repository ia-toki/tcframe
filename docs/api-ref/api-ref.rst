.. _api-ref:

API Reference
=============

- :ref:`Problem configuration <api-ref_problem-config>`
- :ref:`Input/output variables <api-ref_io-variables>`
- :ref:`Input/output segments <api-ref_io-segments>`
- :ref:`Constraints <api-ref_constraints>`
- :ref:`Generator configuration <api-ref_generator-config>`
- :ref:`Test cases <api-ref_test-cases>`
- :ref:`Random number generator <api-ref_rnd>`
- :ref:`Command line options <api-ref_cli-options>`

----

.. _api-ref_problem-config:

Problem configuration
---------------------

The following methods are available inside the overridden method :code:`BaseProblem::Config()`.

.. cpp:function:: void setSlug(string slug)

    Sets the *slug* of the problem. A slug is a nickname or code for the problem. The produced test case filenames will have the slug as prefix. For example, if the slug is "helloworld" then one valid test case filename is "helloworld_1.in".

    If not specified, the default slug is :code:`"problem"`.

.. cpp:function:: void setTimeLimit(int timeLimitInSeconds)

    Sets the time limit of the problem, in seconds. This time limit is used in :ref:`submission simulation <submission>`.

.. cpp:function:: void setMemoryLimit(int memoryLimitInMegabytes)

    Sets the memory limit of the problem, in MB. This memory limit is used in submission simulation.

.. cpp:function:: void setMultipleTestCasesCount(int& countVariable)

    Sets **countVariable** to hold the total number of test cases in a single file, in :ref:`multiple test cases per file <multi-case>` problems.

----

.. _api-ref_io-variables:

Input/output variables
----------------------

There are three supported types of variables:

Scalar
    Variables of built-in integral types (:code:`int`, :code:`long long`, :code:`char`, etc.), built-in floating-point types (:code:`float`, :code:`double`), and :code:`std::string`.

Vector
    :code:`std::vector<T>`, where :code:`T` is a scalar type as defined above. Arrays (:code:`T[]`) are not supported.

Matrix
    :code:`std::vector<std::vector<T>>`, where :code:`T` is a scalar type as defined above. 2D arrays (:code:`T[][]`) are not supported.

----

.. _api-ref_io-segments:

Input/output segments
---------------------

The following macros are available inside the overridden method :code:`BaseProblem::InputFormat()` and :code:`BaseProblem::OutputFormat()`.

.. py:function:: EMPTY_LINE()

    Defines an empty line.

.. py:function:: LINE(comma-separated elements)

    Defines a single line containing space-separated scalar or vector variables. In case of vector variables, the elements are separated by spaces as well.

    **element** is one of:

    - *<scalar variable name>*.
    - *<vector variable name>* **% SIZE(**\ *<number of elements>*\ **)**. The number of elements can be a constant or a scalar variable.
    - *<vector variable name>*. Here, the number of elements is unspecified. This kind of element must occur last in a line segment, if any. Elements will be considered until new line is found.

    For example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINE(N);
            LINE(A % SIZE(3), B);
            LINE(M, C % SIZE(M));
        }

    With **N** = 2, **A** = {1, 2, 3}, **B** = {100, 200, 300, 400}, **M** = 2, **C** = {7, 8}, the above segments will produce:

    ::

        2
        1 2 3 100 200 300 400
        2 7 8

.. py:function:: LINES(comma-separated vector/matrix variable names) % SIZE(number of elements)

    Defines multiple lines, each consisting space-separated elements of given vector/matrix variables.

    For example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINES(V) % SIZE(2);
            LINES(X, Y) % SIZE(N);
        }

    With **V** = {1, 2}, **X** = {100, 110, 120}, **Y** = {200, 210, 220}, **N** = 3, the above segments will produce:

    ::

        1
        2
        100 200
        110 210
        120 220

    If a matrix variable is given, it must occur as the last argument, and the number of rows must match with the number of elements of the other vector variables (if any). It is not required that each row of the matrix consists of the same number of columns.

    For example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINES(op, data) % SIZE(2);
        }

    With **op** = {"UPDATE, "QUERY"}, **data** = {{3, 5}, {7}}, the above segments will produce:

    ::

        UPDATE 3 5
        QUERY 7

.. py:function:: GRID(matrix variable name) % SIZE(number of rows, number of columns)

    Defines a grid consisting elements of a given matrix variable. If the given matrix variable is of type char, the elements in each row is not space-separated, otherwise they are space-separated.

    For example:

    .. sourcecode:: cpp

        void InputFormat() {
            GRID(G) % SIZE(2, 2);
            GRID(H) % SIZE(R, C);
        }

    With **G** = {{'a', 'b'}, {'c', 'd'}}, **H** = {{1, 2, 3}, {4, 5, 6}}, **R** = 2, **C** = 3, the above segments will produce:

    ::

        ab
        cd
        1 2 3
        4 5 6

----

.. _api-ref_constraints:

Constraints
-----------

The following macros are available inside the overridden method :code:`BaseProblem::Constraints()`, :code:`BaseProblem::MultipleTestCasesConstraints()`, and :code:`BaseProblem::SubtaskX()`.

.. py:function:: CONS(predicate)

    Defines a constraint. **predicate** is a boolean expression, whose value must be completely determined by the values of the input variables (only).

    For example:

    .. sourcecode:: cpp

        void Subtask1() {
            CONS(A <= B && B <= 1000);
            CONS(graphDoesNotHaveCycles());
        }

----

.. _api-ref_generator-config:

Generator configuration
-----------------------

The following methods are available inside the overridden method :code:`BaseGenerator::Config()`.

.. cpp:function:: void setTestCasesDir(string testCasesDir)

  Sets the directory for the generated test case files, relative to the location of the generator program.

  If not specified, the default directory is :code:`"tc"`.

.. cpp:function:: void setSolutionCommand(string solutionCommand)

  Sets the command for executing the official solution. This will be used for generating test case output files. For
  each input files, this will be executed:

  .. sourcecode:: bash

      solutionCommand < [input filename] > [output filename]

  If not specified, the default solution command is :code:`"./solution"`.

----

.. _api-ref_test-cases:

Test cases
----------

The following macros are available inside the overridden method :code:`BaseGenerator::TestCases()`.

.. cpp:function:: void assignToSubtasks(set<int> subtaskNumbers)

    Assigns the current test test group to a set of subtasks.

    For example:

    .. sourcecode:: cpp

        void TestGroup1() {
            assignToSubtasks({1, 3});

            // test case definitions follow
        }

The following macros are available inside the overridden method :code:`BaseGenerator::TestCases()` and :code:`BaseGenerator::TestGroupX()`.

.. py:function:: CASE(comma-separated statements)

    Defines a test case.

    **statement** should be one of:

    - assignment to an input variables
    - private method call that assigns values to one or more input variables

    For example:

    .. sourcecode:: cpp

        void TestCases() {
            CASE(N = 42, M = 100, randomArray());
            CASE(N = 1000, M = 1000, randomArray());
        }

The following macros are available inside the overridden method :code:`BaseGenerator::SampleTestCases()`.

.. py:function:: SAMPLE_CASE(list of lines, [list of subtask numbers])

    Defines a sample test case. A sample test case is defined as an exact literal string, given as list of lines. **list of subtask numbers** are only valid in problems with subtasks.

    For example, to define this sample test case:

    ::

        1 2
        3 4 5

    You can do this way:

    .. sourcecode:: cpp

        void SampleTestCases() {
            SAMPLE_CASE({
                "1 2",
                "3 4 5"
            });
        }

    for problems without subtasks. For problems with subtasks:

    .. sourcecode:: cpp

        void SampleTestCases() {
            SAMPLE_CASE({
                "1 2",
                "3 4 5"
            }, {1, 3});
        }

    assuming that the sample test case is assigned to subtasks 1 and 3.

    Multiple sample test cases can be defined inside the same method.

----

.. _api-ref_rnd:

Random number generator
-----------------------

The following methods are available on the random number generator :code:`rnd` object inside a generator.

.. cpp:function:: int nextInt(int minNum, int maxNum)

    Returns a uniformly distributed random integer (int) between **minNum** and **maxNum**, inclusive.

.. cpp:function:: int nextInt(int maxNumEx)

    Returns a uniformly distributed random integer (int) between 0 and **maxNumEx** - 1, inclusive.

.. cpp:function:: long long nextLongLong(long long minNum, long long maxNum)

    Returns a uniformly distributed random integer (long long) between **minNum** and **maxNum**, inclusive.

.. cpp:function:: long long nextLongLong(long long maxNumEx)

    Returns a uniformly distributed random integer (long long) between 0 and **maxNumEx** - 1, inclusive.

.. cpp:function:: double nextDouble(double minNum, double maxNum)

    Returns a uniformly distributed random real number (double) between **minNum** and **maxNum**, inclusive.

.. cpp:function:: double nextDouble(double maxNum)

    Returns a uniformly distributed random real number (double) between 0 and **maxNum**, inclusive.

.. cpp:function:: void shuffle(std::RandomAccessIterator first, std::RandomAccessIterator last)

    Randomly shuffles the elements in [\ **first**, **last**). Use this rather than :code:`std::random_shuffle()`.

----

.. _api-ref_cli-options:

Command-line options
--------------------

The following options can be specified when running the runner program. They mostly override the specified problem and generator configuration.

.. py:function:: --slug=slug

    Overrides the slug specified by :code:`setSlug()` in problem configuration.

.. py:function:: --tc-dir=dir

    Overrides the test cases directory specified by :code:`setTestCasesDir()` in generator configuration.

.. py:function:: --solution-command=command

    Overrides the solution command specified by :code:`setSolutionCommand()` in generator configuration.

.. py:function:: --seed=seed

    Sets the seed for the random number generator :code:`rnd` inside the generator.

.. py:function:: --time-limit=timeLimitInSeconds

    Overrides the time limit specified by :code:`setTimeLimit()` in problem configuration.

.. py:function:: --memory-limit=memoryLimitInMegabytes

    Overrides the memory limit specified by :code:`setMemoryLimit()` in problem configuration.

.. py:function:: --no-time-limit

    Unsets the time limit specified by :code:`setTimeLimit()` in problem configuration.

.. py:function:: --no-memory-limit

    Unsets the memory limit specified by :code:`setMemoryLimit()` in problem configuration.

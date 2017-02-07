.. _api-ref:

API Reference
=============

.. contents::
   :local:

----

.. _api-ref_problem-spec:

Problem spec
------------

``ProblemSpec`` must inherit ``tcframe::BaseProblemSpec``:

.. sourcecode:: cpp

    class ProblemSpec : public BaseProblemSpec {};

Except for private helper functions, every member of ``ProblemSpec`` listed below must be ``protected``.

----

.. _api-ref_io-variables:

Input/output variables
**********************

Defined as instance member variables of ``ProblemSpec``, which will be referenced in other methods of ``ProblemSpec`` and ``TestSpec``.

There are three supported types of variables:

Scalar
    Variables of built-in integral types (``int``, ``long long``, ``char``, etc.), built-in floating-point types (``float``, ``double``), and ``std::string``.

Vector
    ``std::vector<T>``, where ``T`` is a scalar type as defined above. Arrays (``T[]``) are not supported.

Matrix
    ``std::vector<std::vector<T>>``, where ``T`` is a scalar type as defined above. 2D arrays (``T[][]``) are not supported.

Example:

.. sourcecode:: cpp

    class ProblemSpec : public BaseProblemSpec {
    protected:
        int A, B;
        vector<int> parent;
        vector<vector<int>> values;
    };

----

.. _api-ref_io-formats:

Input/output formats
********************

.. sourcecode:: cpp

    virtual void InputFormat() = 0;

Defines the input format. It is mandatory.

.. sourcecode:: cpp

    virtual void BeforeOutputFormat() {}

Executed right before the produced output is validated against the output format. See :ref:`io-formats_before-output-format` for more details.

.. sourcecode:: cpp

    virtual void OutputFormat() {}

Defines the output format. It is optional; if not implemented, then the output will not be validated.

**Defining format**

The following macros are exposed to define input/output formats:

.. py:function:: EMPTY_LINE()

    Defines an empty line.

.. py:function:: RAW_LINE(string variable name)

    Defines a line of raw string. The variable must be of ``std::string`` type.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            RAW_LINE(S);
        }

    With **S** = "Hello, world!", the above format will produce:

    ::

        Hello, world!

.. py:function::
    RAW_LINES(vector of string variable name)
    RAW_LINES(vector of string variable name) % SIZE(number of elements)

    Defines multiple lines, each consisting of raw string. The variable must be of ``std::vector<std::string>`` type.

    If the size is not given, then this must be the last segment in the I/O format.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            RAW_LINES(X) % SIZE(2);
            RAW_LINES(Y);
        }

    With **X** = {"Hello, world!", "Happy new year."}, **Y** = {"lorem", "ipsum", "dolor sit amet"}, the above format will produce:

    ::

        Hello, world!
        Happy new year.
        lorem
        ipsum
        dolor sit amet

.. py:function:: LINE(comma-separated elements)

    Defines a single line containing space-separated scalar or vector variables. In case of vector variables, the elements are separated by spaces as well.

    **element** is one of:

    - *<scalar variable name>*.
    - *<vector variable name>* **% SIZE(**\ *<number of elements>*\ **)**. The number of elements can be a constant or a scalar variable.
    - *<vector variable name>*. Here, the number of elements is unspecified. This kind of element must occur last in a line segment, if any. Elements will be considered until new line is found.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINE(N);
            LINE(A % SIZE(3), B);
            LINE(M, C % SIZE(M));
        }

    With **N** = 2, **A** = {1, 2, 3}, **B** = {100, 200, 300, 400}, **M** = 2, **C** = {7, 8}, the above format will produce:

    ::

        2
        1 2 3 100 200 300 400
        2 7 8

.. py:function::
    LINES(comma-separated vector/matrix variable names)
    LINES(comma-separated vector/matrix variable names) % SIZE(number of elements)

    Defines multiple lines, each consisting of space-separated elements of given vector/matrix variables.

    If the size is not given, this must be the last segment in the I/O format.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINES(V) % SIZE(2);
            LINES(X, Y) % SIZE(N);
            LINES(Z);
        }

    With **V** = {1, 2}, **X** = {100, 110, 120}, **Y** = {200, 210, 220} **N** = 3, **Z** = {1, 2, 3, 4} the above format will produce:

    ::

        1
        2
        100 200
        110 210
        120 220
        1
        2
        3
        4

    If a matrix variable is given, it must occur as the last argument, and the number of rows must match with the number of elements of the other vector variables (if any). It is not required that each row of the matrix consists of the same number of columns.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            LINES(op, data) % SIZE(2);
        }

    With **op** = {"UPDATE", "QUERY"}, **data** = {{3, 5}, {7}}, the above format will produce:

    ::

        UPDATE 3 5
        QUERY 7

.. py:function:: GRID(matrix variable name) % SIZE(number of rows, number of columns)

    Defines a grid consisting elements of a given matrix variable. If the given matrix variable is of type ``char``, the elements in each row is not space-separated, otherwise they are space-separated.

    Example:

    .. sourcecode:: cpp

        void InputFormat() {
            GRID(G) % SIZE(2, 2);
            GRID(H) % SIZE(R, C);
        }

    With **G** = {{'a', 'b'}, {'c', 'd'}}, **H** = {{1, 2, 3}, {4, 5, 6}}, **R** = 2, **C** = 3, the above format will produce:

    ::

        ab
        cd
        1 2 3
        4 5 6

----

.. _api-ref_styles:

Problem styles
**************

.. sourcecode:: cpp

    virtual void StyleConfig() {}

Defines the options to enable for problem styles. The following methods are exposed:

.. cpp:function:: CustomScorer()

    Declares that the problem needs a custom scorer.

.. cpp:function:: NoOutput()

    Declares that the problem does not need test case output files.

See :ref:`styles` for more details.

Example:

.. sourcecode:: cpp

    void StyleConfig() {
        CustomScorer();
        NoOutput();
    }

----

.. _api-ref_constraints:

Constraints and subtasks
************************

.. sourcecode:: cpp

    virtual void MultipleTestCasesConstraints() {}

Defines the constraints to be imposed to the multiple test cases counter.

.. sourcecode:: cpp

    virtual void Constraints() {}

Defines the constraints to be imposed to the :ref:`input/output variables <api-ref_io-variables>`.

.. sourcecode:: cpp

    virtual void Subtask1() {}
    virtual void Subtask2() {}
    // ...
    virtual void Subtask25() {}

Defines the constraints to be imposed to the :ref:`input/output variables <api-ref_io-variables>` for each subtask (up to 25).

**Defining constraints**

The following macro is exposed to define constraints:

.. py:function:: CONS(predicate)

    Defines a constraint. **predicate** is a boolean expression, whose value must be completely determined by the values of the input variables (only).

    Example:

    .. sourcecode:: cpp

        void Subtask1() {
            CONS(A <= B && B <= 1000);
            CONS(graphDoesNotHaveCycles());
        }

----

.. _api-ref_multi-case-config:

Multiple test cases config
**************************

.. sourcecode:: cpp

    virtual void MultipleTestCasesConfig() {}

Defines the config for :ref:`multiple test cases per file problems <multi-case>`. The following methods are exposed:

.. cpp:function:: Counter(int& var)

    Sets the input variable that will hold the number of test cases in a file.

.. cpp:function:: OutputPrefix(std::string prefix)

    Sets the prefix to be prepended to the output of each test case. It can include ``%d``, which will be replaced by the actual test case number (1-based).

Example:

.. sourcecode:: cpp

    void MultipleTestCasesConfig() {
        Counter(T);
        OutputPrefix("Case #%d: ");
    }

----

.. _api-ref_grading-config:

Grading config
**************

.. sourcecode:: cpp

    virtual void GradingConfig() {}

Defines the config for :ref:`local grading <grading>`. The following methods are exposed:

.. cpp:function:: TimeLimit(int timeLimitInSeconds)

    Sets the time limit in seconds. If not specified, the default value is 2 seconds.

.. cpp:function:: MemoryLimit(int memoryLimitInMegabytes)

    Sets the memory limit in MB. If not specified, the default value is 64 MB.

Example:

.. sourcecode:: cpp

    void GradingConfig() {
        TimeLimit(3);
        MemoryLimit(256);
    }

----

.. _api-ref_test-spec:

Test spec
---------

``TestSpec`` must inherit ``tcframe::BaseTestSpec<ProblemSpec>``:

.. sourcecode:: cpp

    class TestSpec : public BaseTestSpec<ProblemSpec> {};

Except for private helper functions, every member of ``TestSpec`` listed below must be ``protected``.

----

.. _api-ref_sample-test-cases:

Sample test cases
*****************

.. sourcecode:: cpp

    virtual void SampleTestCase1() {}
    virtual void SampleTestCase2() {}
    // ...
    virtual void SampleTestCase25() {}

Defines the sample test cases (up to 25). The following methods are exposed:

.. cpp:function:: Subtasks(std::set<int> subtaskNumbers)

    Assigns the current sample test case to a set of subtasks, if the problem has :ref:`subtasks <subtasks>`. If used, this should be the first call in a sample test case.

.. cpp:function:: Input(std::vector<std::string> lines)

    Defines the input as exact literal string, given as list of lines.

.. cpp:function:: Output(std::vector<std::string> lines)

    Defines the input as exact literal string, given as list of lines. It is optional; if not specified, the solution will be run against the sample input to produce the corresponding sample output.

Example:

.. sourcecode:: cpp

    void SampleTestCase1() {
        Input({
            "4 6",
            "a b c"
        });
        Output({
            "10"
        });
    }

----

.. _api-ref_test-cases:

Test cases and test groups
**************************

.. sourcecode:: cpp

    virtual void TestCases() {}

Defines the test cases.

.. sourcecode:: cpp

    virtual void TestGroup1() {}
    virtual void TestGroup2() {}
    // ...
    virtual void TestGroup25() {}

Defines the test cases on each test group (up to 25). The following method is exposed:

.. cpp:function:: Subtasks(std::set<int> subtaskNumbers)

   Assigns the current test group to a set of :ref:`subtasks <subtasks>`. This should be the first call in a test group.

   .. sourcecode:: cpp

       void TestGroup1() {
           Subtasks({1, 3});

           // test case definitions follow
       }

**Defining test cases**

The following macro is exposed to define test cases:

.. py:function:: CASE(comma-separated statements)

    Defines a test case.

    **statement** should be one of:

    - assignment to an input variables
    - private method call that assigns values to one or more input variables

    Example:

    .. sourcecode:: cpp

        void TestCases() {
            CASE(N = 42, M = 100, randomArray());
            CASE(N = 1000, M = 1000, randomArray());
            CASE(randomEqualNandM(), randomArray());
        }

----

.. _api-ref_test-case-lifecycle:

Test case lifecycle
*******************

.. sourcecode:: cpp

    virtual void BeforeTestCase() {}
    virtual void AfterTestCase() {}

Hook up additional logic to run during in a :ref:`test case lifecycle <test-cases_lifecycle>`.

For each test case, the following things will happen in order:

#. ``BeforeTestCase()`` is executed.
#. The assignments/method calls inside ``CASE()`` are executed.
#. ``AfterTestCase()`` is executed.
#. Input variable values are printed according to the input format.

----

.. _api-ref_rnd:

Random number generator
-----------------------

``BaseTestSpec`` exposes a random number generator object ``rnd`` that can be utilized to define test cases. The following methods are available on it:

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

    Randomly shuffles the elements in [\ **first**, **last**). Use this instead of :code:`std::random_shuffle()`.

----

.. _api-ref_runner:

Runner program
--------------

A runner is the compiled version of a spec file, and is capable of two things:

Test cases generation
*********************

::

    ./runner [options]

.. py:function:: --output=<dir>

    The output directory to which the test cases will be generated. Default: ``tc``.

.. py:function:: --solution=<command>

    The solution command to use for generating output files. Default: ``./solution``.

.. py:function:: --scorer=<command>

    The custom scorer command to use for checking sample output strings in problem spec class. Default: ``./scorer``.

.. py:function:: --seed=<seed>

    The seed for random number generator ``rnd`` in the test spec. Default: ``0``.

Local grading
*************

::

    ./runner grade [options]

.. py:function:: --output=<dir>

    The output directory from which the generated test cases will be read. Default: ``tc``.

.. py:function:: --solution=<command>

    The solution command to grade. Default: ``./solution``.

.. py:function:: --scorer=<command>

    The custom scorer command to use. Default: ``./scorer``.

.. py:function:: --time-limit=<time-limit-in-seconds>

    Overrides the time limit specified by ``TimeLimit()`` in grading config.

.. py:function:: --memory-limit=<memory-limit-in-megabytes>

    Overrides the memory limit specified by ``MemoryLimit()`` in grading config.

.. py:function:: --no-time-limit

    Unsets the time limit specified by ``TimeLimit()`` in grading config.

.. py:function:: --no-memory-limit

    Unsets the memory limit specified by ``MemoryLimit()`` in grading config.

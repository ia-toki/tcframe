.. _test-cases:

Test cases
==========

Technically, a **test case** is a particular state of input variables, with the corresponding correct values for the output variables. If the values of the input variables conform to each of the specified constraints, then the test case is considered valid, otherwise it is invalid.

Test cases are defined in the ``TestCases()`` method of the test spec class.

.. sourcecode:: cpp

    void TestCases() {
        // list of test case definitions
    }

A **sample test case** is similar to a test case, except that it is not secret and appears in the problem statement. Sample test cases are defined in the ``SampleTestCaseX()`` methods of the test spec class, where X is the sample test case number.

.. sourcecode:: cpp

    void SampleTestCase1() {
        // sample test case I/O definitions
    }

----

Test case definition
--------------------

Test cases are specified as a list of **test case definitions**. Each test case definition is a comma-separated list of input variable assignments. The list is passed as the argument to the ``CASE()`` macro. The macros are then called one-by-one as method calls:

.. sourcecode:: cpp

    void TestCases() {
        CASE(<assignments list 1>);
        CASE(<assignments list 2>);
        ...
    }

For example, a valid test case definition is ``CASE(N = 100, M = 75)``. Vectors and matrices can be assigned conveniently using braces; for example, ``CASE(V = {1, 2, 3}, M = {{1, 2}, {3, 4}})``.

For more complex assignment, such as constructing a tree, create a private ``void`` method that constructs the tree, and call it in ``CASE()``. For example, ``CASE(N = 100, linearTree())`` and having this private method:

.. sourcecode:: cpp

    /* Constructs a linear tree */
    void linearTree() {
        for (int i = 0; i < N; i++) {
            parent.push_back(i-1);
        }
    }

.. note::

    The values of input variables before each test case are **undefined**, and may contain leftover computation from previous test cases. See :ref:`test-cases_lifecycle` on how to initialize them.

You can also use for-loop construct for assigning some input variables. This is usually done if you want to exhaust all valid possibilities of some input variables. For example:

.. sourcecode:: cpp

    void TestCases() {
        for (int i = 1; i <= 10; i++) {
            CASE(N = i);
        }
    }

We recommend not using this style unless absolutely necessary.

Each test case definition will be realized as a pair of input/output files, with the following filenames: ``<slug>_<case-number>.in`` and ``<slug>_<case-number>.out``. For example, ``sum_1.in``, ``sum_1.out``.

----

Sample test case definition
---------------------------

.. note::

    As of this version, you can define up to 25 sample test casses: **SampleTestCase1()** .. **SampleTestCase25()**.

Unlike test cases, sample test cases are specified as literal strings, exactly as they appear in problem statement. This way, you can be visually sure that you are typing sample test cases in problem statement correctly.

The literal strings are passed line-by-line to the ``Input()`` and ``Output()`` calls as follows (note the ``{}`` braces):

.. sourcecode:: cpp

    void SampleTestCase1() {
        Input({
            "<line 1>",
            "<line 2>",
            ...
        });
        Output({
            "<line 1>",
            "<line 2>",
            ...
        });
    }

The supplied output must match with the actual output produced by the solution program.

For example, this sample test case:

Input:

::

    3 4
    1 2 3

Output:

::

    2
    100
    200

can be translated to:

.. sourcecode:: cpp

    void SampleTestCase1() {
        Input({
            "3 4",
            "1 2 3"
        });
        Output({
            "2",
            "100",
            "200"
        });
    }

.. note::

    The ``Output()`` part of a sample test case definition is optional, and if not present, the solution will be run to produce the output. However, it is only for easier migration from **tcframe** 0.x. You should always specify both input and output, so that you are sure you are typing the output correctly in the problem statement by only looking at the spec file (no need to check with the actual produced output file).

    Of course, if ``NoOutput()`` is enabled (see :ref:`styles`), then ``Output()`` is not allowed to be specified.

:ref:`If your problem has subtasks <subtasks>`, you also need to assign each sample test case to a set of subtasks, by calling ``Subtasks()`` at the beginning of each ``SampleTestCaseX()`` with the set of subtask numbers, as follows.

.. sourcecode:: cpp

    void SampleTestCase1() {
        Subtasks({2, 3});
        Input({
            "3 4",
            "1 2 3"
        });
        Output({
            "2",
            "100",
            "200"
        });
    }

Each sample test case will be realized as a pair of input/output files, with the following filenames: ``<slug>_sample_<case-number>.in`` and ``<slug>_sample_<case-number>.out``. For example, ``sum_sample_1.in``, ``sum_sample_1.out``.

----

Random number generator
-----------------------

**tcframe** provides a simple random number generator object, ``tcframe::rnd``. For example, you can use it to generate a random array: ``CASE(N = 100, randomArray())`` where ``randomArray()`` is defined as follows.

.. sourcecode:: cpp

    void randomArray() {
        for (int i = 0; i < N; i++) {
            A.push_back(rnd.nextInt(1000000));
        }
    }

For more details, consult the :ref:`API reference for random number generator <api-ref_rnd>`.

----

.. _test-cases_lifecycle:

Test case lifecycle
-------------------

.. note::

    This section only applies to official test cases. It is not applicable to sample test cases.

**tcframe** declares two methods that you can implement in the test spec class: ``BeforeTestCase()`` and ``AfterTestCase()`` to hook up additional logic to run during a test case generation. For each test case, the following things will happen in order:

#. ``BeforeTestCase()`` is executed.
#. The assignments/method calls inside ``CASE()`` are executed.
#. ``AfterTestCase()`` is executed.
#. Input variable values are printed according to the input format.

BeforeTestCase()
****************

This method can be implemented to initialize input variables. For example, if you have vectors as input variables, you will want to initialize them first before calling e.g. ``randomArray()`` inside ``CASE()`` macro.

.. sourcecode:: cpp

    void BeforeTestCase() {
        parent.clear();
        A.clear();
    }

AfterTestCase()
***************

You may want to manipulate input variables with a different representation from what is defined in the input format section. For example, suppose that you want to have a tree as an input. In the input format (in problem spec class), you specify the tree as a list of edges (**U**\ [i], **V**\ [i]) as follows:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N);
        LINES(U, V) % SIZE(N - 1);
    }

and you want to manipulate the tree as a vector **P**\ [], where **P**\ [i] is the parent of node i. (I.e., you have private variable ``vector<int> P`` in the test spec class.)

This can be achieved by implementing ``AfterTestCase()``, transforming the vector **P**\ [] into a pair of vectors (**U**\ [], **V**\ []) there.

.. sourcecode:: cpp

    void AfterTestCase() {
        U.clear();
        V.clear();
        for (int i = 0; i < N; i++) {
            if (P[i] != -1) {
                U.push_back(i);
                V.push_back(P[i]);
            }
        }
    }

.. _test-cases:

Test cases
==========

A **test case** is a state of input variables. If the values of the input variables conform to each of the specified constraints, then the test case is considered valid, otherwise it is invalid.

Test cases are defined in the :code:`TestCases()` method of the problem specification class.

.. sourcecode:: cpp

    void TestCases() {
        // list of test case definitions
    }

A **sample test case** is similar to a test case, except that it is not secret and appears in the problem statement. Sample test cases are defined in the :code:`SampleTestCases()` method of the problem specification class.

.. sourcecode:: cpp

    void SampleTestCases() {
        // list of sample test case definitions
    }

----

Test case definition
--------------------

Test cases are specified as a list of **test case definitions**. Each test case definition is a comma-separated list of input variable assignments. The list is passed as the argument to the :code:`CASE()` macro. The macros are then called one-by-one as method calls:

.. sourcecode:: cpp

    void TestCases() {
        CASE(<assignments list 1>);
        CASE(<assignments list 2>);
        ...
    }

For example, a valid test case definition is :code:`CASE(N = 100, M = 75)`. Vectors and matrices can be assigned conveniently using braces; for example, :code:`CASE(V = {1, 2, 3}, M = {{1, 2}, {3, 4}})`.

For more complex assignment, such as constructing a tree, create a private :code:`void` method that constructs the tree, and call it in :code:`CASE()`. For example, :code:`CASE(N = 100, linearTree())` and having this private method:

.. sourcecode:: cpp

    /* Constructs a linear tree */
    void linearTree() {
        parent.clear(); // important!
        for (int i = 0; i < N; i++) {
            parent.push_back(i-1);
        }
    }

.. note::

    The values of input variables before each test case are **undefined**, and may contain leftover computation from previous test cases. For example, for vectors, make sure that you initialize or clear them before adding values, as we did in the above example!

You can also use for-loop construct for assigning some input variables. This is usually done if you want to exhaust all valid possibilities of some input variables. For example:

.. sourcecode:: cpp

    void TestCases() {
        for (int i = 1; i <= 10; i++) {
            CASE(N = i);
        }
    }

We recommend not using this style unless absolutely necessary.

Each test case definition will be realized as a pair of input/output files, with the following filenames: :code:`<slug>_<case-number>.in` and :code:`<slug>_<case-number>.out`. For example, :code:`aplusb_1.in`, :code:`aplusb_1.out`.

----

Sample test case definition
---------------------------

Unlike test cases, sample test cases are specified as literal strings, exactly as they appear in problem statement. This way, you can be visually sure that you are typing sample test cases in problem statement correctly.

The literal strings are passed line-by-line to the :code:`SAMPLE_CASE()` macro as follows (note the :code:`{}` braces):

.. sourcecode:: cpp

    void SampleTestCases() {
        // Sample test case 1
        SAMPLE_CASE({
            "<line 1>",
            "<line 2>",
            ...
        });

        // Sample test case 2
        SAMPLE_CASE({
            "<line 1>",
            "<line 2>",
            ...
        });

        ...
    }

For example, these sample test cases:

::

    3 4
    1 2 3

and

::

    6 2
    10 1 4 5 7 3

can be translated to:

.. sourcecode:: cpp

    void SampleTestCases() {
        SAMPLE_CASE({
            "3 4",
            "1 2 3"
        });

        SAMPLE_CASE({
            "6 2",
            "10 1 4 5 7 3"
        });

        ...
    }

Each sample test case definition will be realized as a pair of input/output files, with the following filenames: :code:`<slug>_sample_<case-number>.in` and :code:`<slug>_sample_<case-number>.out`. For example, :code:`aplusb_sample_1.in`, :code:`aplusb_sample_1.out`.

----

Random number generator
-----------------------

**tcframe** provides a simple random number generator object, :code:`tcframe::rnd`. For example, you can use it to generate a random array: :code:`CASE(N = 100, randomArray())` where :code:`randomArray()` is defined as follows.

.. sourcecode:: cpp

    void randomArray() {
        A.clear();
        for (int i = 0; i < N; i++) {
            A.push_back(rnd.nextInt(1000000));
        }
    }

For more details, consult the :ref:`API reference for random number generator<api-ref_rnd>`.

----

Manipulating input variables with different representation
----------------------------------------------------------

Often, you want to manipulate input variables with a different representation from what is defined in the input format section. For example, suppose that you want to have a tree as an input. In the input format (in problem specification class), you specify the tree as a list of edges (**U**\ [i], **V**\ [i]) as follows:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N);
        LINES(U, V) % SIZE(N - 1);
    }

and you want to manipulate the tree as a vector **P**\ [], where **P**\ [i] is the parent of node i. (I.e., you have private variable :code:`vector<int> P` in generator specification class.)

This can be achieved by writing a special method :code:`FinalizeInput()` in generator specification class and transforming the vector **P**\ [] into a pair of vectors (**U**\ [], **V**\ []) in it.

.. sourcecode:: cpp

    void FinalizeInput() {
        U.clear();
        P.clear();
        for (int i = 0; i < N; i++) {
            if (P[i] != -1) {
                U.push_back(i);
                V.push_back(P[i]);
            }
        }
    }

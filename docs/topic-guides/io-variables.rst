.. _io-variables:

I/O variables
=============

**Input variables** are C++ variables whose values compose test cases input files. They are are direct translations from variables mentioned in the problem statement's input format section into C++ variables. For example, given the following description:

    The first line contains two integers **N** and **K**. The second line contains **N** space-separated integers **A**\ [1], **A**\ [2], ..., **A**\ [\ **N**\ ].

we will have three input variables: ``int N``, ``int K``, and ``vector<int> A``.

Input variables are the **basic units** of computation in **tcframe**. For example, they are manipulated in three different places:

- :ref:`Input format definition <io-formats>` in problem spec. It determines the layout of test case input files.
- :ref:`Constraint definitions <constraints>` in problem spec. They determine whether the values of input variables are valid.
- :ref:`Test case definitions <test-cases>` in test spec. Each of them determines the content of a single test case input file.

**Output variables** are defined in a similar way, the only difference being that they are just manipulated in output format definition. Also, usually they consist of just a single variable without name in the problem statement's output format. We can just name them ``result`` or something similar.

I/O variables are defined as protected member variables in the problem spec class. For example:

.. sourcecode:: cpp

    class ProblemSpec : public BaseProblemSpec {
    protected:
        int N, K;
        vector<int> A;

        string result;

        // the rest of problem spec components follow
    };

----

Supported I/O variable types
----------------------------

**tcframe** supports three types of I/O variables as follows.

Scalar
    Variables of built-in integral types (``int``, ``long long``, ``char``, etc.), built-in floating-point types (``float``, ``double``), and ``std::string``.

Vector
    ``std::vector<T>``, where ``T`` is a scalar type as defined above.

Matrix
    ``std::vector<std::vector<T>>``, where ``T`` is a scalar type as defined above.

Other types are not supported as I/O variables. **tcframe** prefers STL types whenever possible. For example, ``char*`` is not supported as strings. Also, regular arrays (``T[]``) and 2D arrays (``T[][]``) are not supported.

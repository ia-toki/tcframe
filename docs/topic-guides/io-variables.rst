.. _io-variables:

I/O variables
=============

**Input variables** are C++ variables whose values compose test cases input files. They are are direct translations from variables mentioned in the problem statement's input format section into C++ variables. For example, given the following description:

    The first line contains two integers **N** and **K**. The second line contains **N** space-separated integers **A**\ [1], **A**\ [2], ..., **A**\ [\ **N**\ ].

we will have three input variables: :code:`int N`, :code:`int K`, and :code:`vector<int> A`.

Input variables are the **basic units** of computation in **tcframe**. For example, they are manipulated in three different places:

- :ref:`Input format definition <io-formats>` in problem specification. It determines the layout of test case input files.
- :ref:`Constraint definitions <constraints>` in problem specification. They determine whether the values of input variables are valid.
- :ref:`Test case definitions <test-cases>` in generator specification. Each of them determines the content of a single test case input file.

**Output variables** are defined in a similar way, the only difference being that they are just manipulated in output format definition. Also, usually they consist of just a single variable without name in the problem statement's output format. We can just name them :code:`result` or something similar.

I/O variables are defined as protected member variables in the problem specification class. For example:

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:
        int N, K;
        vector<int> A;

        string result;

        // the rest of problem specification components follow
    };

----

Supported I/O variable types
----------------------------

**tcframe** supports three types of I/O variables as follows.

Scalar
    Variables of built-in integral types (:code:`int`, :code:`long long`, :code:`char`, etc.), built-in floating-point types (:code:`float`, :code:`double`), and :code:`std::string`.

Vector
    :code:`std::vector<T>`, where :code:`T` is a scalar type as defined above.

Matrix
    :code:`std::vector<std::vector<T>>`, where :code:`T` is a scalar type as defined above.

Other types are not supported as I/O variables. **tcframe** prefers STL types whenever possible. For example, :code:`char*` is not supported as strings. Also, regular arrays (:code:`T[]`) and 2D arrays (:code:`T[][]`) are not supported.
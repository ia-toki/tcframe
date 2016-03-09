.. _io-formats:

I/O formats
===========

**Input format** specifies two things at the same time:

- How the input variable values should be printed to official test case input files.
- How the sample test case input files should be parsed into input variables, as they are specified as literal strings. If some sample sample test cases can't be parsed, it means that you have written invalid sample test cases, and the whole generation will fail.

**Output format** specifies how the output files should be parsed into output variables. If the output files can't be parsed, it means that you have written invalid solution program, and the whole generation will fail.

Each of the I/O formats is defined as a list of **I/O segments** in :code:`InputFormat()` and :code:`OutputFormat()` methods of the problem specification class, respectively, as follows.

.. sourcecode:: cpp

    void InputFormat() {
        // list of input segments
    }

    void OutputFormat() {
        // list of output segments
    }


Each I/O segment is specified by a function-like C++ macro. The I/O segments are specified one-by-one, each as a statement that calls the macro. For example:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N, M);
        EMPTY_LINE();
        LINE(A % SIZE(N));
    }

----

Supported I/O segments
----------------------

The list of supported segments is given below. In all segments, it must be noted that:

- The terms scalar, vector, and matrix below refer to the :ref:`I/O variable types described in the previous chapter <io-variables>`.
- Vectors and matrices will have 0-based indexing. This is strict and there is no way to enforce 1-based indexing.

Empty line
    Specified by the macro :code:`EMPTY_LINE()`. Represents a single, empty line.

Singe line
    Specified by the macro :code:`LINE(...)`. Represents space-separated values in a single line.

    The macro accepts one or more I/O variables as arguments. The value of the variables will be space-separated. For example: "... the first line consists of two space-separated integers **N** and **M**" can be specified by :code:`LINE(N, M)`.

    Besides scalars, the arguments can be vectors, too. You need to specify the number of elements using :code:`% SIZE(<size>)` syntax after each vector. For example: "... the next line consists of **N** space-separated integers **A**\ [0], **A**\ [1], ... **A**\ [\ **N**-1]" can be specified by :code:`LINE(A % SIZE(N))`. The size can be a constant as well; for example, :code:`LINE(B % SIZE(3))`.

    You can freely combine scalars and vectors in :code:`LINE()`; for example, :code:`LINE(N, A % SIZE(N), C % SIZE(2))` would represent a line consisting of :code:`N A[0] A[1] ... A[N-1] C[0] C[1]`.

    Lastly, you can specify a vector without fixed size, as long as it is the last argument. For example: "... the input consists an integer **X** followed by space-separated integers" can be specified by :code:`LINE(X, A)` (assuming :code:`A` is a vector).

Multiple lines
    Specified by the macro :code:`LINES(...) % SIZE(<size>)`. Represents multiple lines, each containing an element of a vector given as the argument.

    For example: "... each of the next **N** lines contains an integer **X**\ [i]" can be specified by :code:`LINES(X) % SIZE(N)`.

    There could be more than one vector as well. For example: "... the next **N** lines each contains space-separated integers **X**\ [i] and **Y**\ [i]" can be specified by :code:`LINES(X, Y) % SIZE(N)`. If there are multiple vectors, they all must have the same number of elements.

    You also could specify jagged vector as the last argument. This is useful for input format like, "... then **M** lines follow. Each line begins with a string **op**. If **op** is :code:`UPDATE`, then it is followed by two integers **x** **y**. If it is :code:`QUERY`, then it is followed by a single integer **z**". This can be specified by :code:`LINES(op, data)` where :code:`op` is a :code:`vector<string>` and :code:`data` is a :code:`vector<vector<int>>`. Then, :code:`data[i]` represents a vector of integers in the i-th line, which can consist of one of two elements.

Grid
    Specified by the macro :code:`GRID(...) % SIZE(<row>, <column>)`. Represents a grid containing elements of a matrix given as the argument, laid out in a grid.

    For example: "... each fo the next **R** lines contain **C** integers **G**\ [i][j]" can be specified by :code:`GRID(G) % SIZE(R, C)`.

    If the matrix is of type :code:`char`, the elements in each row is not space-separated, otherwise they are space-separated.

For more details, consult the :ref:`API reference for I/O segments <api-ref_io-segments>`.

----

Notes
-----

Unfortunately, the following are not supported (yet):

Constants in I/O segments
    For example: "... the first line will always consist of the string :code:`BEGIN`." Everything must be wrapped in variables.

    As a workaround, just create an input variable and initialize it to :code:`BEGIN`.

Complex conditional I/O format that can't be handled by jagged vectors
    There is **NO** known general workaround yet. We're still working on designing how to handle complex format.

    However, there are workarounds for simple cases, for example:

    "Output the required sum, or the string :code:`IMPOSSIBLE` if there is no solution."

    In this case, you can just use a string as the output variable. The downside is that it is not type-safe; for example, the generation won't fail if the reference solution mistakenly output an invalid string such as :code:`123abc`.

However, a last resort for a workaround does exist for output format. If you have complex output format, you can just omit the method :code:`OutputFormat()` altogether and your solution's output won't be checked at all for validity.
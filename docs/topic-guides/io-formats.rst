.. _io-formats:

I/O formats
===========

**Input format** specifies two things at the same time:

- How the input variable values should be printed to official test case input files.
- How the sample test case input files should be parsed into input variables, as they are specified as literal strings. If some sample sample test cases can't be parsed, it means that you have written invalid sample test cases, and the whole generation will fail.

**Output format** specifies how the output files should be parsed into output variables. If the output files can't be parsed, it means that you have written invalid solution program, and the whole generation will fail.

Each of the I/O formats is defined as a list of **I/O segments** in ``InputFormat()`` and ``OutputFormat()`` methods of the problem specification class, respectively, as follows.

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

There are two categories of I/O segments: raw segments and tokenized segments.

Raw segments
************

These segments do not do any special formatting to the value of the variables; they will print/parse them as-is.

Empty line
    Specified by the macro ``EMPTY_LINE()``. Represents a single, empty line.

Raw line
    Specified by the macro ``RAW_LINE(...)``. Represent a line of the string given as the only argument. The argument must be of ``std::string`` type.

    For example: "... the next line consists of a sentence containing arbitrary printable ASCII characters **S**" can be specified by  ``RAW_LINE(S)`` (where ``S`` is a string).

Multiple raw lines
    Specified by the macro ``RAW_LINES(...) % SIZE(<size>)``. Represent lines of string given as the only argument. The argument must be of ``std::vector<std::string>`` type.

    For example: "... each of the next N lines consists of a sentence containing arbitrary printable ASCII characters **S**" can be specified by  ``RAW_LINES(S) % SIZE(N)`` (where ``S`` is a vector of strings).

    The size can also be omitted, as long as this is the last segment.

Tokenized segments
******************

These segments do some formatting to the value of the variables. For example, the elements of a vector variable in a line will be printed space-separated.

.. note::

    In tokenized segments, string variables cannot have whitespaces. For example, ``hello, world!`` is considered to have two string variables: ``hello,`` and ``world!``. Use raw segments if you want to work with strings containing whitespaces.

Single line
    Specified by the macro ``LINE(...)``. Represents space-separated values in a single line.

    The macro accepts one or more I/O variables as arguments. The value of the variables will be space-separated. For example: "... the first line consists of two space-separated integers **N** and **M**" can be specified by ``LINE(N, M)``.

    Besides scalars, the arguments can be vectors, too. You need to specify the number of elements using ``% SIZE(<size>)`` syntax after each vector. For example: "... the next line consists of **N** space-separated integers **A**\ [0], **A**\ [1], ... **A**\ [\ **N**-1]" can be specified by ``LINE(A % SIZE(N))``. The size can be a constant as well; for example, ``LINE(B % SIZE(3))``.

    You can freely combine scalars and vectors in ``LINE()``; for example, ``LINE(N, A % SIZE(N), C % SIZE(2))`` would represent a line consisting of ``N A[0] A[1] ... A[N-1] C[0] C[1]``.

    Lastly, you can specify a vector without fixed size, as long as it is the last argument. For example: "... the input consists an integer **X** followed by space-separated integers" can be specified by ``LINE(X, A)`` (where ``A`` is a vector).

Multiple lines
    Specified by the macro ``LINES(...) % SIZE(<size>)``. Represents multiple lines, each containing an element of a vector given as the argument.

    For example: "... each of the next **N** lines contains an integer **X**\ [i]" can be specified by ``LINES(X) % SIZE(N)``.

    There could be more than one vector as well. For example: "... the next **N** lines each contains space-separated integers **X**\ [i] and **Y**\ [i]" can be specified by ``LINES(X, Y) % SIZE(N)``. If there are multiple vectors, they all must have the same number of elements.

    You also could specify jagged vector as the last argument. This is useful for input format like, "... then **M** lines follow. Each line begins with a string **op**. If **op** is ``UPDATE``, then it is followed by two integers **x** **y**. If it is ``QUERY``, then it is followed by a single integer **z**". This can be specified by ``LINES(op, data)`` where ``op`` is a ``vector<string>`` and ``data`` is a ``vector<vector<int>>``. Then, ``data[i]`` represents a vector of integers in the i-th line, which can consist of one or two elements.

    Lastly, the size can also be omitted, as long as this is the last segment.

Grid
    Specified by the macro ``GRID(...) % SIZE(<row>, <column>)``. Represents a grid containing elements of a matrix given as the only argument, laid out in a grid.

    For example: "... each fo the next **R** lines contain **C** integers **G**\ [i][j]" can be specified by ``GRID(G) % SIZE(R, C)``.

    If the matrix is of type ``char``, the elements in each row is not space-separated, otherwise they are space-separated.

For more details, consult the :ref:`API reference for I/O formats <api-ref_io-formats>`.

----

Notes
-----

Unfortunately, the following are not supported (yet):

Constants in I/O segments
    For example: "... the first line will always consist of the string ``BEGIN``." Everything must be wrapped in variables.

    As a workaround, just create an input variable and initialize it to ``BEGIN``.

Complex conditional I/O format that can't be handled by jagged vectors
    There is **NO** known general workaround yet. We're still working on designing how to handle complex format.

    However, there are workarounds for simple cases, for example:

    "Output the required sum, or the string ``IMPOSSIBLE`` if there is no solution."

    In this case, you can just use a string as the output variable. The downside is that it is not type-safe; for example, the generation won't fail if the reference solution mistakenly output an invalid string such as ``123abc``.

However, a last resort for a workaround does exist for output format. If you have complex output format, you can just omit the method ``OutputFormat()`` altogether and your solution's output won't be checked at all for validity.

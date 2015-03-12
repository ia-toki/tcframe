Basic concepts
==============

Writing generator program
-------------------------

To write a generator using **tcframe** framework, we need to define two specifications: a *problem specification* and a
*generator specification*. Both specifications are to be written in a file called *generator program*.

The generator program must include **tcframe**'s main header, as follows:

.. sourcecode:: cpp

    #include "tcframe/tcframe.hpp"
    using namespace tcframe;

Problem specification
~~~~~~~~~~~~~~~~~~~~~

A problem specification is defined by writing a class that inherits **BaseProblem**. For simplicity, just name the class
**Problem**.

Here is a template of a problem specification.

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:

        // component specifications

    };

A problem specification consists of several components:

Configuration
*************

This component is defined by overriding the method **BaseProblem::Config()**. Inside this method, you can call:

- **setSlug(string slug)**

  Sets the *slug* of the problem. A slug is a nickname or code for the problem. The produced test case filenames will
  have the slug as prefix. For example, if the slug is **helloworld** then one valid test case filename is
  **helloworld_1.in**.

  If not specified, the default slug is **problem**.

For example:

.. sourcecode:: cpp

    void Config() {
        setSlug("helloworld");
    }

Input variables
***************

Everything that will be included in test case input files must be declared as variables. The variables have to be
declared as member fields inside the **protected** (not **private**) section of the problem class.

There are three types of variables that are supported:

- **Scalar**

  Variables of built-in integral types (**int**, **long long**, **char**, etc.), built-in floating-point types
  (**float**, **double**), and **std::string**.

- **Vector**

  **std::vector<T>**, where **T** is a scalar type as defined above.

- **Matrix**

  **std::vector<std::vector<T>>**, where **T** is a scalar type as defined above.


Input format
************

This component specifies how the input variables should be printed in test case input files. The format is specified in
terms of consecutive input **segment**\ s. The segments are declared inside an overriden **BaseProblem::InputFormat()**
method.

There are three types of segments that are supported:

- **Line segment**

  Defines a single line containing space-separated scalar or vector variables. In case of vector variables, the elements
  are separated by spaces as well.

  To define a line segment, use **LINE()** macro as follows:

  .. sourcecode:: cpp

      LINE([comma-separated elements]);

  where an *element* is one of:

  - *<scalar variable name>*.
  - *<vector variable name>* **% SIZE(**\ *<number of elements>*\ **)**. The number of elements can be constant or a
    scalar variable.

  To specify an empty line, use **EMPTY_LINE()** macro instead.

  For example:

  .. sourcecode:: cpp

      void InputFormat() {
          LINE(N);
          EMPTY_LINE();
          LINE(A % SIZE(N));
          LINE(M, B % SIZE(M));
      }

  With N = 2, A = {1, 2, 3}, M = 2, B = {7, 8}, the above segments will produce:

  ::

      2

      1 2 3
      2 7 8

- **Lines segment**

  Defines multiple lines, each consisting space-separated elements of given vector variables.

  To define a lines segment, use **LINES()** macro as follows:

  .. sourcecode:: cpp

      LINES([comma-separated vector variable names]) % SIZE([number of elements]);

  For example:

  .. sourcecode:: cpp

      void InputFormat() {
          LINES(V) % SIZE(2);
          LINES(X, Y) % SIZE(N);
      }

  With V = {1, 2}, X = {100, 110}, Y = {200, 210}, N = 2, the above segments will produce:

  ::

      1
      2
      100 200
      110 210

- **Grid segment**

  Defines a grid consisting elements of a given matrix variable. If the given matrix variable is of type **char**, the
  elements in each row is not space-separated, otherwise they are space-separated.

  To define a grid segment, use **GRID()** macro as follows:

  .. sourcecode:: cpp

      GRID([matrix variable name]) % SIZE([number of rows], [number of columns]);

  For example:

  .. sourcecode:: cpp

      void InputFormat() {
          GRID(G) % SIZE(2, 2);
          GRID(H) % SIZE(R, C);
      }

  With G = {{'a', 'b'}, {'c', 'd'}}, H = {{1, 2, 3}, {4, 5, 6}}, R = 2, C = 3, the above segments will produce:

  ::

      ab
      cd
      1 2 3
      4 5 6

Constraints
***********

This components specifies the constraints of the problem; i.e., the conditions that must be satisfied by the input
variables. There are two ways for defining constraints:

- **Without subtasks**

  Override the method **BaseProblem::Constraints()**.

- **With subtasks**

  Override each of the methods **BaseProblem::SubtaskX()**, where **X** is a positive integer denoting the subtask
  number. For the current version, **X** can only be at most 10.

Inside the overriden method(s), you can define the constraints. A constraint is defined with a **CONS()** macro as
follows:

.. sourcecode:: cpp

    CONS([predicate]);

where *predicate* is a boolean expression.

For example:

.. sourcecode:: cpp

    void Constraints() {
        CONS(1 <= N && N <= 100);
        CONS(1 <= A && A <= 1000);
        CONS(A <= B && B <= 1000);
        CONS(doesNotHaveCycles());
    }

.. note::
    As of this version, there is no easy way to define a predicate on each of the elements of a vector or matrix. Use
    for-loop instead as a workaround:

    .. sourcecode:: cpp

        void Constraints() {
            CONS(1 <= N && N <= 100);

            for (int i = 0; i < N; i++) {
                CONS(1 <= P[i] <= 1000000000);
            }
        }

Generator specification
~~~~~~~~~~~~~~~~~~~~~~~

A generator specification is defined by writing a class that inherits **BaseGenerator<T>**, where **T** is a problem
specification class (which inherits **BaseProblem**). For simplicity, just name the class **Generator**.

Here is a template of a generator specification.

.. sourcecode:: cpp

    class Generator : public BaseGenerator<Problem> {
    protected:

        // component specifications

    };

A generator specification consists of several components:

Configuration
*************

This component is defined by overriding the method **BaseGenerator::Config()**. Inside this method, you can call:

- **setBaseDir(string directoryName)**

  Sets the directory for the generated test case files, relative to the location of the generator program.

  If not specified, the default directory name is **tc**.

- **setSolution(string solutionExecutionCommand)**

  Sets the command for executing the official solution. This will be used for generating test case output files. For
  each input files, this will be executed:


  .. sourcecode:: bash

      solutionExecutionCommand < [input filename] > [output filename]

  If not specified, the default command is **./solution**.

For example:

.. sourcecode:: cpp

    void Config() {
        setBaseDir("testdata");
        setSolution("java Solution");
    }

Test cases
**********

This component specifies the values of the problem's input variables, for each test case. There are two ways for
defining constraints:

- **For problems without subtasks**

  Override the method **BaseGenerator::TestCases()**. Inside this method, you can define the test cases. A test case
  is defined with a **CASE()** macro as follows:

  .. sourcecode:: cpp

      CASE([comma-separated statement]);

  where a *statement* is either an assignment or function call. Each CASE() defines a single test case and should
  assign valid values to all input variables.

  For example:

  .. sourcecode:: cpp

      void TestCases() {
          CASE(N = 100, A = 1, B = 20);
          CASE(N = rand() % 100, A = rand() % N, B = A * 2);
      }

- **For problems with subtasks**

  If the corresponding problem has subtasks, test cases should be divided into test groups. A test group is a set of
  test cases that are assigned to the same set of subtasks. For example, suppose that there are 3 subtasks, with this
  property: if a solution solves subtask i, it will automatically solve subtask j for all j < i as well. This means
  a test case that is assigned to subtask i, must be assigned to subtask j for all j > i as well.

  Using the concept of test groups, we can define the test cases as follows:

  - Test Group 1: assigned to subtasks 1, 2, and 3
  - Test Group 2: assigned to subtasks 2 and 3
  - Test Group 3: assigned to subtask 3

  To define test groups, override each of the methods **BaseGenerator::TestGroupX()**, where **X** is a positive integer
  denoting the test group number. For the current version, **X** can only be at most 10. Then, call
  **assignToSubtasks(S)** method as the first statement, where **S** is a list of subtask numbers. For example:

  .. sourcecode:: cpp

      void TestGroup1() {
          assignToSubtasks({1, 2, 3});

          CASE(N = 100, A = 1, B = 20);
          CASE(N = rand() % 100, A = rand() % N, B = A * 2);
      }

For both cases (with and without subtasks), you can also specify sample test cases. To specify them, override the method
**BaseGenerator::SampleTestCases()**. Inside this method, you can define sample test cases. A sample test case is
defined with a **SAMPLE_CASE()** macro as follows:

.. sourcecode:: cpp

    SAMPLE_CASE([list of lines]);

for problem without subtasks, and

.. sourcecode:: cpp

    SAMPLE_CASE([list of lines], [list of subtask numbers]);

for problem with subtasks.

Here, a sample test case is not defined by assigning values to the input variables. Instead, it is defined as an exact
literal string, given as list of lines. This is so that we can be sure that the sample test case mentioned in problem
statement and in the generator program match.

For example suppose we want to define sample test case:

::

    1 2
    3 4 5

We can define that in the following way:

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

Main function
*************

After specifying problem and generator, write the **main()** function as follows:

.. sourcecode:: cpp

    int main() {
        Generator gen;
        return gen.generate();
    }

Compiling generator program
---------------------------

Suppose that your generator program is **generator.cpp**. Compile it using this compilation command:

.. sourcecode:: bash

    g++ -I[path to tcframe]/include -std=c++11 -o generator generator.cpp

Running generator program
-------------------------

Just run

.. sourcecode:: bash

    ./generator

The status of the generation of each test case will be output to the standard output. For each successful test cases,
the input-output file pair will be stored in the specified base directory (by default, it is **tc**).

Generation can fail due to several reasons:

- **Invalid input format**

  In this case, no test cases will be generated.  For example: using scalar variable for a grid segment.

- **Invalid input variable states**

  For example: a grid segment requires that the size is 2 x 3, but after applying the test case definition, the matrix
  consists of 3 x 4 elements.

- **Unsatisfied constraints/subtasks**

  The input variables do not conform to the constraints.
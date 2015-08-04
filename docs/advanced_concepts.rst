Advanced concepts
=================

Manipulating input variables with different representation
----------------------------------------------------------

Often, we want to manipulate input variables with a different representation from what is defined in the input format section. For example, suppose that we want to have a tree as an input. In the input format, we specify the tree as a list of edges (U[i], V[i]) as follows:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N);
        LINES(U, V) % SIZE(N - 1);
    }

and we want to manipulate the tree as a vector P[], where P[i] is the parent of node i. (I.e., we have private variable vector<int> P in Generator.)

This can be achieved by overriding a special method **BaseGenerator::FinalizeInput()** method and transforming vector P[] into pair of vectors (U[], V[]) in it.

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

Complex input/output format
---------------------------

Suppose we have the following input format description from the problem statement:

    The first line contains an integer N. The next N lines each contains one of the following type:

    - 1 A[i] B[i]
    - 2 A[i] B[i] C[i]

How to deal with this one?

Unfortunately, as of this version, the above format is not supported yet. This version only supports input/output formats that are free of custom loops and conditional branches.

I am designing how to support that in the next versions.

Complex predicates in constraints
---------------------------------

Similarly, methods used in defining constraints (**Constraints()**, **SubtaskX()**) must be free of custom loops and conditional branches. If you want to use complex predicate, such as determining whether the input is a tree, create a custom private helper method that return boolean (whether the input is a tree).

Multiple test cases per file (ICPC-style)
------------------------------------------------

tcframe supports ICPC-style problem as well. In this style, for each of the **SampleTestCases()**, **TestCases()**, and **TestGroupX()** methods, the test cases will be combined into a single file. The file is prepended with a single line containing the number of test cases in it.

To write, an ICPC-style test cases generator, first write the runner as usual, assuming that the problem not ICPC-style. Then, apply the following changes.

Problem specification class
***************************

- Declare an integer variable (e.g., **T**) that will hold the number of test cases in a single file.

  .. sourcecode:: cpp

      protected:
          int T;

          ...

- In the problem configuration, call **setMultipleTestCasesCount()** method with the previous variable as the argument.

  .. sourcecode:: cpp

      void Config() {
          ...

          setMultipleTestCasesCount(T);

          ...
      }

- The input format specification should not contain **T**. It should specify the format of a single test case only. The number of test cases will be automatically prepended in the final combined test case input file.

- We can impose a constraint on **T**, inside **MultipleTestCasesConstraints()** method.

  .. sourcecode:: cpp

      void MultipleTestCasesConstraints() {
          CONS(1 <= T <= 20);
      }

Generation specification class
******************************

No changes are necessary.

Solution program
****************

Although the input format only specifies a single test case, the solution should read the number of test cases in the first line. In other words, the solution will read the final combined test cases input file.

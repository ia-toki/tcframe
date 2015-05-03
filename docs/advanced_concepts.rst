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

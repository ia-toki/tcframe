Advanced concepts
=================

BaseGenerator::FinalizeInput() intermediate method
--------------------------------------------------

Often, we want to manipulate input variables with a data structure that is significantly different from what are
required as input format. For example, suppose that we want to have a tree as an input. In the input format, we specify
the tree as a list of edges (U[i], V[i]) as follows:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N);
        LINES(U, V) % SIZE(N - 1);
    }

and we want to manipulate the tree as a vector P[], where P[i] is the parent of node i.

This can be solved by overriding **BaseGenerator::FinalizeInput()** method and transform vector P[] into pair of vectors
(U[], V[]) in it.

.. sourcecode:: cpp

    void FinalizeInput() {
        for (int i = 0; i < N; i++) {
            if (P[i] != -1) {
                U.push_back(i);
                V.push_back(P[i]);
            }
        }
    }
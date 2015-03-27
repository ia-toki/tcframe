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

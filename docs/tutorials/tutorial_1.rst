Tutorial 1: Best Pair
=====================

In this tutorial, you will learn how to write a generator for a simple yet tricky problem, called **Best Pair**. We will focus on the general idea of how to come up with a strong set of test cases.

Here is the complete problem statement:

----

| **Best Pair**
| Time Limit: 1 second
| Memory Limit: 64 MB
|
| **Description**
| You are given an array A consisting of N integers. What is the maximum product (multiplication) of any pair of integers in A?
|
| **Input Format**
| The first line contains a single integer N. The next line contains N space-separated elements of A.
|
| **Output Format**
| A single line containing the maximum product.
|
| **Sample Input**
| ``4``
| ``4 -2 3 1``
|
| **Sample Output**
| ``12``
|
| **Constraints**
| 2 ≤ N ≤ 100,000
| -1,000,000 ≤ A[i] ≤ 1,000,000
|

----

Cool, let's now start writing generator for this problem. The conventional steps to do this are:

#. Prepare the problem package.
#. Write the solution.
#. Write the problem spec.
#. Write the test spec.

We will tackle each step in more details below.

.. note::

    The complete source files for this tutorial can also be found `here <https://github.com/tcframe/tcframe-examples/tree/master/tutorials/1_best-pair>`_.

----

Preparing problem package
-------------------------

First, we need to come up with a slug for this problem. Let's call it ``best-pair``.

Now, create a directory ``best-pair`` in your system. For example, create it in your home directory (``~/best-pair``). We will then store all files related to this problem there.

----

Writing solution
----------------

Although the solution can actually be written any time during this process, we recommend doing it **before** writing the spec. Why? Sometimes, after (trying) to write the solution, you will realize that:

- Some constraints make the problem impossible to solve.
- You find a much simpler solution that makes the problem too easy and not suitable for you contest.
- The problem is actually impossible to solve.

Thus, you will save your time from writing the spec if the problem is actually invalid as explained above.

Anyway, let's now solve this problem. The solution is simple: it is the maximum of the following:

- product of two largest non-negative elements
- product of two smallest negative elements
- (**tricky**) product of the largest negative element and the smallest positive element

Here is a sample C++ implementation of the above idea. It is not the fastest solution, but should suffice for the purposes of this tutorial.

``best-pair/solution.cpp``:

.. sourcecode:: cpp

    #include <bits/stdc++.h>
    using namespace std;

    int N;
    vector<long long> pos;
    vector<long long> neg;

    int main() {
        cin >> N;
        for (int i = 0; i < N; i++) {
            long long x;
            cin >> x;
            if (x >= 0) {
                pos.push_back(x);
            } else {
                neg.push_back(x);
            }
        }
        sort(pos.begin(), pos.end());
        sort(neg.begin(), neg.end());

        long long res = LLONG_MIN;
        if (pos.size() >= 2) {
            res = max(res, pos[pos.size()-1] * pos[pos.size()-2]);
        }
        if (neg.size() >= 2) {
            res = max(res, neg[0] * neg[1]);
        }
        if (!neg.empty() && !pos.empty()) {
            res = max(res, neg[neg.size()-1] * pos[0]);
        }

        cout << res << endl;
    }

----

Writing problem spec
--------------------

The following is a problem spec that is derived directly from the problem statement. Note that we are using a private method ``eachElementBetween()`` to represent the constraint -1,000,000 ≤ A[i] ≤ 1,000,000. Everything else should be straightforward.

``best-pair/spec.cpp``:

.. sourcecode:: cpp

    #include <bits/stdc++.h>
    #include <tcframe/spec.hpp>

    using namespace std;
    using namespace tcframe;

    class ProblemSpec : public BaseProblemSpec {
    protected:
        int N;
        vector<long long> A;
        long long res;

        void InputFormat() {
            LINE(N);
            LINE(A % SIZE(N));
        }

        void OutputFormat() {
            LINE(res);
        }

        void GradingConfig() {
            TimeLimit(1);
            MemoryLimit(64);
        }

        void Constraints() {
            CONS(2 <= N && N <= 100000);
            CONS(eachElementBetween(A, -1000000, 1000000));
        }

    private:
        bool eachElementBetween(const vector<long long>& v, long long lo, long long hi) {
            for (long long x : v) {
                if (x < lo || x > hi) {
                    return false;
                }
            }
            return true;
        }
    };

----

Writing test spec
-----------------

This is the most challenging part!

First, write the sample test cases as written in the problem statement as follows. As an advice, sample test cases should not reveal all trickiness of the problem. In the sample below, the answer is the most obvious case: the product of two largest non-negative elements of A.

``best-pair/spec.cpp`` (continued):

.. sourcecode:: cpp

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        void SampleTestCase1() {
            Input({
                "4",
                "4 -2 3 1"
            });
            Output({
                "12"
            });
        }
    };

Then, let's move on to the actual, official test cases. Firstly, we will implement the ``BeforeTestCase()`` method to initialize the vector A before every test case, as follows.

.. sourcecode:: cpp

    void BeforeTestCase() {
        A.clear();
    }

Next, the actual test cases. We should come up with a good set of test cases that does both the following:

- accepting correct solutions, and
- rejecting incorrect solutions (**important!**)

Recall that the solution we've written above considers 3 different cases. Let's write a test case that covers each case.

1. The answer is the product of two largest non-negative elements
    ``CASE(N = 5, A = {-2, -1, 2, 3, 0});``

2. The answer is the product of two smallest negative elements
    ``CASE(N = 5, A = {3, 4, -1, -3, -5});``

3. The answer is the product of the largest negative element and the smallest positive element
    ``CASE(N = 2, A = {2, -1});``

    Note that the above case is only possible with N = 2.

There are also several cases for which the answer has some interesting properties, as follows.

4. The answer is 0
    This is the case when everything but one is zero, or everything is zero.

    ``CASE(N = 4, A = {0, 2, 0, 0});``

    ``CASE(N = 4, A = {0, 0, 0, 0});``

5. The answer is the maximum possible answer
    This will reject solutions that do not use 64-bit integers.

    ``CASE(N = 3, A = {1000000, -1, 1000000});``

6. The answer is the minimum possible answer
    This will reject solutions that do not use 64-bit integers as well.

    ``CASE(N = 2, A = {-1000000, 1000000});``

    Note that (3) and (6) will reject solutions which initialize the answer to 0 (instead of a very small negative integer).

So far, we have considered various cases for which the answer has some properties. Let's now consider cases for which the **input** itself has some interesting properties.

7. All elements are positive/negative
    This will reject solutions which do not properly handle empty collections for positive/negative elements.

    ``CASE(N = 4, A = {1, 2, 3, 4});``

    ``CASE(N = 4, A = {-1, -2, -3, -4});``

8. All elements are zero
    This can probably reject solutions which separate zeroes for some reasons (it's actually unnecessary).

    This is already covered by (4).

9. N = 2
    This is the smallest possible value for N. Already covered by (3) and (6).

10. Random values for elements of A
     It's always good idea to include randomized input to the test data when the input space is very large (which should be true for most problems).

     The randomized elements of A can be generated using a private function, as follows:

     .. sourcecode:: cpp

         void randomElements() {
             for (int i = 0; i < N; i++) {
                 A.push_back(rnd.nextInt(-1000000, 1000000));
             }
         }

     (Note that at the beginning of the above method, N will have been set to a value from ``CASE()``, and A will have been cleared by ``BeforeTestCase()``.)

     The above is good enough for this problem. However, it is nicer if we can somehow "tune" some properties of the randomization. For example, we can have parameters denoting the number of desired positive and negative numbers in A:


     .. sourcecode:: cpp

         void randomElements(int numPos, int numNeg) {
             assert(numPos + numNeg <= N);

             for (int i = 0; i < numPos; i++) {
                 A.push_back(rnd.nextInt(1, 1000000));
             }
             for (int i = 0; i < numNeg; i++) {
                 A.push_back(rnd.nextInt(-1000000, -1));
             }
             for (int i = 0; i < N-numPos-numNeg; i++) {
                 A.push_back(0);
             }

             rnd.shuffle(A.begin(), A.end());
         }

     Again, the above tuning is not really necessary for this problem, as most tricky cases have been covered by previous hand-made test cases. However, for the purpose of learning, we will still use the tuning.

     It is not the only tuning that we can do. Other options include:

     - Tuning the range of possible values of the randomized element. For example, if we want the elements to be randomized between just 1 and 100.
     - Same as above, but also tuning the percentage of the tuned range. For example, if we want the elements to be totally randomized, except a quarter of them, which should be between 1 and 100.
     - etc.

     All right, now use this function in ``CASE()``, with various arguments to it (and various values for N), for example as follows.

     ``CASE(N = 10, randomElements(5, 5));``

     ``CASE(N = 100, randomElements(20, 50));``

     ``CASE(N = 1000, randomElements(300, 300));``

     ``CASE(N = 10000, randomElements(2500, 6000));``


11. N = 100000
     The maximum value of N. This will reject solutions that use array with size less than 100,000.

     ``CASE(N = 100000, randomElements(50000, 50000));``

     ``CASE(N = 100000, randomElements(10000, 80000));``

     ``CASE(N = 100000, randomElements(80000, 10000));``

There are possibly some other cases that we can explore, but for now, this set of test cases should be strong enough for our problem!

----

Putting it all together
-----------------------

Here is the complete spec file for our Best Pair problem.

.. sourcecode:: cpp

    #include <bits/stdc++.h>
    #include <tcframe/spec.hpp>

    using namespace std;
    using namespace tcframe;

    class ProblemSpec : public BaseProblemSpec {
    protected:
        int N;
        vector<long long> A;
        long long res;

        void InputFormat() {
            LINE(N);
            LINE(A % SIZE(N));
        }

        void OutputFormat() {
            LINE(res);
        }

        void GradingConfig() {
            TimeLimit(1);
            MemoryLimit(64);
        }

        void Constraints() {
            CONS(2 <= N && N <= 100000);
            CONS(eachElementBetween(A, -1000000, 1000000));
        }

    private:
        bool eachElementBetween(const vector<long long>& v, long long lo, long long hi) {
            for (long long x : v) {
                if (x < lo || x > hi) {
                    return false;
                }
            }
            return true;
        }
    };

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        void SampleTestCase1() {
            Input({
                "4",
                "4 -2 3 1"
            });
            Output({
                "12"
            });
        }

        void BeforeTestCase() {
            A.clear();
        }

        void TestCases() {
            // The answer is the product of two largest non-negative elements
            CASE(N = 5, A = {-2, -1, 2, 3, 0});

            // The answer is the product of two smallest negative elements
            // The smallest possible value for N
            CASE(N = 5, A = {3, 4, -1, -3, -5});

            // The answer is the product of the largest negative element and the smallest positive element
            CASE(N = 2, A = {2, -1});

            // The answer is 0
            CASE(N = 4, A = {0, 2, 0, 0});
            CASE(N = 4, A = {0, 0, 0, 0});

            // The answer is the maximum possible answer
            CASE(N = 3, A = {1000000, -1, 1000000});

            // The answer is the minimum possible answer
            CASE(N = 2, A = {-1000000, 1000000});

            // All elements are positive/negative
            CASE(N = 4, A = {1, 2, 3, 4});
            CASE(N = 4, A = {-1, -2, -3, -4});

            // Random values for elements of A
            CASE(N = 10, randomElements(5, 5));
            CASE(N = 100, randomElements(20, 50));
            CASE(N = 1000, randomElements(300, 300));
            CASE(N = 10000, randomElements(2500, 6000));

            // The maximum value of N
            CASE(N = 100000, randomElements(50000, 50000));
            CASE(N = 100000, randomElements(10000, 80000));
            CASE(N = 100000, randomElements(80000, 10000));
        }

    private:
        void randomElements(int numPos, int numNeg) {
            assert(numPos + numNeg <= N);

            for (int i = 0; i < numPos; i++) {
                A.push_back(rnd.nextInt(1, 1000000));
            }
            for (int i = 0; i < numNeg; i++) {
                A.push_back(rnd.nextInt(-1000000, -1));
            }
            for (int i = 0; i < N-numPos-numNeg; i++) {
                A.push_back(0);
            }

            rnd.shuffle(A.begin(), A.end());
        }
    };

That's it! The complete source files for this tutorial can also be found `here <https://github.com/tcframe/tcframe-examples/tree/master/tutorials/1_best-pair>`_.

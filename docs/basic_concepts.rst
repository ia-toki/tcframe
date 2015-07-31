Basic concepts
==============

Let's learn how to write a generator using tcframe with this basic tutorial. Consider the following example problem.

**Description**

You are given integers N and K, and an array consisting of N integers. Compute the largest possible product of any K elements of the array! Each element can be used more than once.

**Input Format**

The first line contains two integers N and K. The second line contains N space-separated integers: the elements of the array.

**Output Format**

A single line containing the required product.

**Sample Input**

::

    5 3
    0 1 -1 2 -2

**Sample Output**

::

    8

**Subtask 1:**

- N = 1
- 1 <= K <= 100
- -100000 <= A[i] <= 100000

**Subtask 2:**

- 1 <= N <= 100
- K = 1
- -100000 <= A[i] <= 100000

**Subtask 3:**

- 1 <= N <= 100
- 1 <= K <= 100
- -100000 <= A[i] <= 100000

We will be writing a test cases generator for this problem. The program we will be writing is called **runner program**. Create a C++ file, for example, **runner.cpp**.

First of all, we need to include the main tcframe runner header file:

.. sourcecode:: cpp

    #include "tcframe/runner.hpp"
    using namespace tcframe;

Then, we will need to write specifications as will be explained in the next sections.

Problem specification
---------------------

The first step is to write a **problem specification** class. It is basically a class that represents a problem's test case variables, I/O format, and the required constraints/subtasks.

Create a class that inherits **BaseProblem**. The convention is to name the class **Problem**:

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:

        // components here

    };

A problem specification consists of several components. Note that publicly defined components must all go to the protected section of the class. You are allowed to write private helper variables and/or methods, of course.

Problem configuration
---------------------

This component consists of configuration of several aspects of the problem. To define this component, override the method **void BaseProblem::Config()**. For example, for this problem, we want that the slug (i.e., file prefix) for the test cases to be "k-product".

.. sourcecode:: cpp

    void Config() {
        setSlug("k-product");
    }

The complete reference of configurable aspects of a problem can be found here: :ref:`Problem configuration API reference <api-ref-problem-configuration>`.

This component can be omitted if you don't want to override any default settings.

Input/output variables
----------------------

**Input variables** are a collection of variables which compose test cases inputs. They can be usually found in the input format section in the problem statement. For this problem, we have three input variables: **N**, **K**, and **A**. The input variables are defined as protected member variables.

In this problem, we have two scalars (**N**, **K**) and one vector (**A**) as the input variables. We define them as follows:

.. sourcecode:: cpp

    int N;
    int K;
    vector<int> A;

Similarly, **output variables** are a collection of variables which compose test cases inputs. Most of the cases, this is just a single variable and does not have a particular name in the problem statement. Let's just call it result.

.. sourcecode:: cpp

    int N;
    int K;
    vector<int> A;

    int result;

The complete reference of input/output variables can be found here: :ref:`Input/output variables API reference <api-ref-io-variables>`.

Input/output format
-------------------

**Input format** specifies how the input variables should be printed in test case input files. To define this component, override the method **void BaseProblem::InputFormat()**. The format is specified in terms of consecutive input **segment**\ s. Basically an input segment arranges the layout of several input variables.

A test case input file for this problem consists of a single containing **N** and **K**, followed by a single line containing space-separated elements of **A**. We can define that format as follows:

.. sourcecode:: cpp

    void InputFormat() {
        LINE(N, K);
        LINE(A % SIZE(N));
    }

Similarly, **output format** specifies how the input variables should be printed in test case input files. To define this component, override the method **void BaseProblem::OutputFormat()**.

.. sourcecode:: cpp

    void OutputFormat() {
        LINE(result);
    }

The complete reference of input/output segments can be found here: :ref:`Input/output segments API reference <api-ref-io-segments>`.

Constraints
-----------

This components specifies the constraints of the problem; i.e., the conditions that must be satisfied by the input/output
variables. Two types of problems are supported: the ones without subtasks, and the ones with subtasks.

**For problems without subtasks**\ : Override the method **void BaseProblem::Constraints()**.

**For problems with subtasks**\ : Override each of the methods **void BaseProblem::SubtaskX()**, where **X** is a positive integer denoting the subtask number.

.. note::

    As of this version, you can define up to 10 subtasks: **Subtask1()** .. **Subtask10()**.

Inside the overriden method(s), we can define the constraints. A constraint is defined with a **CONS()** macro containing a boolean expression.

Let's define the subtasks for this problem.

.. sourcecode:: cpp

    void Subtask1() {
        CONS(N == 1);
        CONS(1 <= K && K <= 100);
        CONS(eachElementBetween(A, -100000, 100000));
    }

    void Subtask2() {
        CONS(1 <= N && N <= 100);
        CONS(K == 1);
        CONS(eachElementBetween(A, -100000, 100000));
    }

    void Subtask3() {
        CONS(1 <= N && N <= 100);
        CONS(1 <= K && K <= 100);
        CONS(eachElementBetween(A, -100000, 100000));
    }

where **eachElementBetween()** is a private helper method, defined as follows:

.. sourcecode:: cpp

    bool eachElementBetween(const vector<int>& A, int lo, int hi) {
        for (int x : A) {
            if (x < lo || x > hi) {
                return false;
            }
        }
        return true;
    }

.. note::

    As of this version, there is currently no easy way to test a predicate for each element of a vector. The workaround is to write a helper method ourselves, like what we did above.

The complete reference of constraints can be found here: :ref:`Constraints API reference <api-ref-constraints>`.

We have now completed writing a problem specification class. In summary, our class should look like this:

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:
        int N;
        int K;
        vector<int> A;

        int result;

        void Config() {
            setSlug("k-product");
        }

        void InputFormat() {
            LINE(N, K);
            LINE(A % SIZE(N));
        }

        void OutputFormat() {
            LINE(result);
        }

        void Subtask1() {
            CONS(N == 1);
            CONS(1 <= K && K <= 100);
            CONS(eachElementBetween(A, -100000, 100000));
        }

        void Subtask2() {
            CONS(1 <= N && N <= 100);
            CONS(K == 1);
            CONS(eachElementBetween(A, -100000, 100000));
        }

        void Subtask3() {
            CONS(1 <= N && N <= 100);
            CONS(1 <= K && K <= 100);
            CONS(eachElementBetween(A, -100000, 100000));
        }

    private:
        bool eachElementBetween(const vector<int>& A, int lo, int hi) {
            for (int x : A) {
                if (x < lo || x > hi) {
                    return false;
                }
            }
            return true;
        }
    };

The nice thing is that this problem specification class is really similar to the problem statement! This class will then serve as a "contract" for the generator, which we will write next.

Generator specification
-----------------------

The next step is to write a **generator specification** class.  It is basically a class that represents a collection of (randomly generated) test cases, based on the specification defined in the problem specification class.

Create a class that inherits **BaseGenerator<T>**, where **T** is the problem specification class. The convention is to name the class **Generator**:

.. sourcecode:: cpp

    class Generator : public BaseGenerator<Problem> {
    protected:

        // components here

    };

Similar to a problem specification, a generator specification consists of several components, which must go to the protected section of the class.

Generator configuration
-----------------------

This component consists of configuration of several aspects of the problem. To define this component, override the method void BaseGenerator::Config(). Currently, we can define where the test cases are output, and which solution to run on the test case input files.

For this problem:

.. sourcecode:: cpp

    void Config() {
        setTestCasesDir("tc");
        setSolutionCommand("./solution");
    }

.. note::

    For this tutorial, please create an executable file named "solution" in the same directory as generator.cpp. It could be any solution -- for example, a solution that just prints Hello World.

The complete reference of generator configuration can be found here: :ref:`Generator configuration API reference <api-ref-generator-configuration>`.

The above configuration is the default one. This component can be omitted if you don't want to override any default values, which we will do for this tutorial.

Test cases
----------

This component specifies a collection of values of the problem's input variables, each of which constitute a test case. Two types of problems are supported: the ones without subtasks, and the ones with subtasks.

**For problems without subtasks:** Override the method **BaseGenerator::TestCases()**. The content of this method will be explained shortly.

**For problems with subtasks:** The idea is that a test case must be able to be assigned to more than one subtasks. To support this, we introduce a concept called **test groups**. A test group is a set of test cases that are assigned to the same set of subtasks.

First, create a Venn diagram denoting the valid test cases for all subtasks. For this problem, the diagram will be like this:

.. image:: venn-diagram.png

In order to have a strong set of test cases, we should create a test group for each **closed region** in the Venn diagram. In this case, we will have four test groups as follows:

- Test group 1: consists of only one test case N = K = 1. Assign it to subtasks {1, 2, 3}.
- Test group 2: generate test cases that satisfy N = 1; 2 <= K <= 100. Assign them to subtasks {1, 3}.
- Test group 3: generate test cases that satisfy 2 <= N <= 100; K = 1. Assign them to subtasks {2, 3}.
- Test group 4: generate test cases that satisfy 2 <= N, K <= 100. Assign them to subtasks {3}.

To define test groups, override each of the methods **BaseGenerator::TestGroupX()**, where **X** is a positive integer denoting the test group number. Then, call **assignToSubtasks(S)** method as the first statement, where **S** is a list of subtask numbers. The remaining content of test group methods are test case definitions which will be explained below.

.. note::

    As of this version, you can define up to 10 test groups: **TestGroup1()** .. **TestGroup10()**.

Inside the methods **TestCases()** or **TestGroupX()**, we can define the test cases. A test case is defined with a **CASE()** macro containing a list of assignment to an input variable or method call. Each CASE() defines a single test case and should assign valid values to all input variables. For example:

.. sourcecode:: cpp

    void TestGroup2() {
        assignToSubtasks({1, 3});

        CASE(N = 1, K = 3, randomArray());
        CASE(N = 1, K = 100, randomArray());
    }

where **randomArray()** is a private helper method that assign random values (between -100000 and 100000) to each of the element A[0] .. A[N-1]:


.. sourcecode:: cpp

    void randomArray() {
        A.clear(); // important!
        for (int i = 0; i < N; i++) {
            A.push_back(rnd.nextInt(-100000, 100000));
        }
    }

.. note::

    Yes, we can access the input variables directly inside the generator, even though they belong to the problem specification class!

Here, we are using random number generator object :code:`rnd` that is available inside generator class. The complete reference of randomization methods can be found here: :ref:`Random number generator API reference <api-ref-random-number-generator>`.

We will also define sample test cases. Each sample test case is independent to each other, and they are not included in any test group. Therefore, for problems with subtasks, we must assign a set of subtasks for each sample test case.

The complete reference of test case and sample test case definitions can be found here: :ref:`Test cases API reference <api-ref-test cases>`.

Main function
-------------

After writing problem and generator specification classes, write the **main()** function and configure the runner as follows:

.. sourcecode:: cpp

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

The complete runner program for this problem is summarized below.

Note that for vector input variables, don't forget to clear them before assigning the values.

.. sourcecode:: cpp

    #include "tcframe/runner.hpp"
    using namespace tcframe;

    #include <random>
    #include <vector>
    using namespace std;

    class Problem : public BaseProblem {
    protected:
        int N;
        int K;
        vector<int> A;

        int result;

        void Config() {
            setSlug("k-product");
        }

        void InputFormat() {
            LINE(N, K);
            LINE(A % SIZE(N));
        }

        void OutputFormat() {
            LINE(result);
        }

        void Subtask1() {
            CONS(N == 1);
            CONS(1 <= K && K <= 100);
            CONS(eachElementBetween(A, -100000, 100000));
        }

        void Subtask2() {
            CONS(1 <= N && N <= 100);
            CONS(K == 1);
            CONS(eachElementBetween(A, -100000, 100000));
        }

        void Subtask3() {
            CONS(1 <= N && N <= 100);
            CONS(1 <= K && K <= 100);
            CONS(eachElementBetween(A, -100000, 100000));
        }

    private:
        bool eachElementBetween(const vector<int>& A, int lo, int hi) {
            for (int x : A) {
                if (x < lo || x > hi) {
                    return false;
                }
            }
            return true;
        }
    };

    class Generator : public BaseGenerator<Problem> {
    protected:
        void SampleTestCases() {
            SAMPLE_CASE({
                "5 3",
                "0 1 -1 2 -2"
            }, {3});
        }

        void TestGroup1() {
            assignToSubtasks({1, 2, 3});

            CASE(N = 1, K = 1, randomArray());
        }

        void TestGroup2() {
            assignToSubtasks({1, 3});

            CASE(N = 1, K = 2, randomArray());
            CASE(N = 1, K = 10, randomArray());
            CASE(N = 1, K = 100, randomArray());
        }

        void TestGroup3() {
            assignToSubtasks({2, 3});

            CASE(N = 2, K = 1, randomArray());
            CASE(N = 10, K = 1, randomArray());
            CASE(N = 100, K = 1, randomArray());
        }

        void TestGroup4() {
            assignToSubtasks({3});

            CASE(N = 2, K = 2, randomArray());
            CASE(N = 10, K = 10, randomArray());
            CASE(N = 42, K = 58, randomArray());
            CASE(N = 100, K = 100, randomArray());
            CASE(N = 100, K = 100, randomArray());
            CASE(N = 100, K = 100, randomArray());
        }

    private:
        void randomArray() {
            A.clear(); // important!
            for (int i = 0; i < N; i++) {
                A.push_back(rnd.nextInt(-100000, 100000));
            }
        }
    };

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

Compiling runner program
---------------------------

Suppose that your runner program is **runner.cpp**. Compile it using this compilation command:

.. sourcecode:: bash

    g++ -I[path to tcframe]/include -std=c++11 -o runner runner.cpp

For example:

.. sourcecode:: bash

    g++ -I/home/fushar/tcframe/include -std=c++11 -o runner runner.cpp

.. note::

    The current version needs GCC version >= 4.7.

Running runner program
----------------------

Just run

.. sourcecode:: bash

    ./runner

There are several command-line options that can be specified when running the runner program. The options mostly override the problem and generator configuration. For example, we can override the specified official solution:

.. sourcecode:: bash

    ./runner --solution-command="java Solution"

See :ref:`Command-line options <api-ref-command-line-options>` for more information on the command-line options.

The status of the generation of each test case will be output to the standard output. For each successful test case,
the input-output file pair will be stored in the specified test cases directory (by default, it is "tc").

Generation can fail due to several reasons:

Invalid input/output format
    For example: using scalar variable for a grid segment.

Invalid input variable states
  For example: a grid segment requires that the size is 2 x 3, but after applying the test case definition, the matrix consists of 3 x 4 elements.

Unsatisfied constraints/subtasks
    The input variables do not conform to the constraints.

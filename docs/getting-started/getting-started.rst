.. _getting-started:

Getting Started
===============

In **tcframe** world, a problem is defined by a **problem package**, which consists of a **spec file** and one or more solution files. The spec file is then compiled into a **runner program**. The runner program together with the compiled solution files can then generate test cases for the problem.

Let's write our first problem package! We are going to write a package for the following infamous "A+B" problem:

| **A+B Problem**
| Time Limit: 1 second
| Memory Limit: 64 MB
|
| **Description**
| You are given two integers A and B. Compute the sum of them!
|
| **Input Format**
| A single line containing two space-separated integers A and B.
|
| **Output Format**
| A single line containing the required sum.
|
| **Sample Input**
| ``4 6``
|
| **Sample Output**
| ``10``
|
| **Constraints**
| - 1 ≤ A ≤ 1,000
| - 1 ≤ B ≤ 1,000
|

.. note::

    This starter guide will just demonstrate the basic features of **tcframe**. For more advanced features, please consult the :ref:`Topic Guides <topic-guides>` afterwards.

----

.. _getting-started_installation:

Installation
------------

Firstly, we must get **tcframe** on our system. It consists of C++ header files and a few helper scripts.

Download the latest **tcframe** `here <https://github.com/tcframe/tcframe/releases/download/v1.2.1/tcframe_1.2.1.zip>`_. Extract the zip file somewhere on your system; for example, ``~/tcframe``. We will call this directory "**tcframe** home". Confirm that you extracted it correctly by verifying that the directory ``include`` exists directly inside **tcframe** home.

Then, add the following lines to your ``~/.bashrc``. This will set the environment variable ``TCFRAME_HOME`` to our **tcframe** home directory, and make ``tcframe`` command available everywhere.

::

    export TCFRAME_HOME=~/tcframe
    alias tcframe=$TCFRAME_HOME/scripts/tcframe


Restart your terminal session. Now, if you run

::

    tcframe version

You should see the following output

::

    usage: tcframe <command>

    Available commands:
      build         Compile spec file into runner program
      version       Print tcframe version

You're good to go!

----

Preparing package directory
---------------------------

Create a directory called ``sum`` somewhere. This will be our package directory for this A+B problem. Store all the files you will be creating on this guide in this directory.

----

Writing solution file
---------------------

In order to be able to generate the test case output files, a reference solution must be available. For the purpose of this guide, call the solution file ``solution.cpp``. Example solution:

.. sourcecode:: cpp

    #include <iostream>
    using namespace std;

    int main() {
        int A, B;
        cin >> A >> B;
        cout << (A + B) << endl;
    }

Then, compile it to ``solution`` executable, for example:

::

    g++ -o solution solution.cpp

----

Writing spec file
-----------------

A spec file contains problem spec and test spec.

Create a C++ source file called ``spec.cpp``. Copy-paste the following code to the file:

.. sourcecode:: cpp

    #include <tcframe/spec.hpp>
    using namespace tcframe;

    class ProblemSpec : public BaseProblemSpec {
    protected:
        int A, B;
        int sum;

        void InputFormat() {
            LINE(A, B);
        }

        void OutputFormat() {
            LINE(sum);
        }

        void GradingConfig() {
            TimeLimit(1);
            MemoryLimit(64);
        }

        void Constraints() {
            CONS(1 <= A && A <= 1000);
            CONS(1 <= B && B <= 1000);
        }
    };

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        void SampleTestCase1() {
            Input({
                "4 6"
            });
            Output({
                "10"
            });
        }

        void TestCases() {
            CASE(A = 1, B = 1);
            CASE(A = 1000, B = 1000);
            CASE(A = 42, B = 100);
            CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
        }
    };

We will explain this spec file in more details later -- keep going!

Building runner program
-----------------------

Next, we will compile this spec file into what we call a **runner program**. We will use the ``tcframe`` command. Simply run this in the ``sum`` directory:

::

    tcframe build

This will compile ``spec.cpp`` into ``runner``. Make sure that it compiles before continuing this getting started guide!

Finally, run the runner program:

::

    ./runner

If everything is OK, you should get the following output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      sum_sample_1: OK

    [ OFFICIAL TEST CASES ]
      sum_1: OK
      sum_2: OK
      sum_3: OK
      sum_4: OK

    Generation finished. All test cases OK.

Congratulations, you have just written your first problem package using **tcframe** framework! Now, check out your ``sum/tc`` directory -- it will contain the generated test case files.

----

Inspecting problem package
--------------------------

We will now examine each component of a problem package in more details.

Slug
****

A slug is a unique name/codename/identifier for the problem. It is taken from name of the problem package directory. Since we call our problem package directory ``sum``, the slug of our example problem is ``sum``.

Spec file
*********

A spec file is a C++ source file called ``spec.cpp`` that lives inside the problem package directory.

tcframe header
**************

.. sourcecode:: cpp

    #include <tcframe/spec.hpp>
    using namespace tcframe;

``tcframe/spec.hpp`` is the main **tcframe**'s header file for spec files. Each component of **tcframe** lives in the ``tcframe`` namespace, just like the STL functions that live in the ``std`` namespace. By importing the namespace, we don't have to explicitly prefix each class/object we want to use with ``tcframe::``.

Problem spec class
******************

.. sourcecode:: cpp

    class ProblemSpec : public BaseProblemSpec {
    protected:
        ...
    };

A **problem spec** class is where we define the I/O formats, constraints, and some configurations of our problem. This class must inherit ``tcframe::BaseProblemSpec``, and must be called ``ProblemSpec``.

All required members of this class must go in the **protected** section.

Grading configuration
*********************

.. sourcecode:: cpp

    void GradingConfig() {
        TimeLimit(1);
        MemoryLimit(64);
    }

Quite self-explanatory. This has actually no effect during test cases generation, and will affect local grading as explained in later section of this guide. If not specified, the default time limit is 2 seconds, and the default memory limit is 64 megabytes.

Input/output variables and formats
**********************************

.. sourcecode:: cpp

    int A, B;
    int sum;

    void InputFormat() {
        LINE(A, B);
    }

    void OutputFormat() {
        LINE(sum);
    }

Next, we defined the input and output variables and formats. The input consists of two values: **A** and **B**. The output consists of one value; let's call it **sum**. We must declare a variable for each of those values, and then tell **tcframe** how to format them in the input/output files.

Here, we declared two integers ``A`` and ``B`` as **input variables**, and an integer ``sum`` as an **output variable**. ``InputFormat()`` and ``OutputFormat()`` methods specify the input/output formats in terms of the input/output variables. The ``LINE()`` macro here specifies a line consisting of space-separated values of the given arguments.

Constraints
***********

.. sourcecode:: cpp

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }

The last part of a problem spec is **constraints** specification.

A constraint must depend on input variables **only**. Each constraint can be specified as a boolean predicate inside the ``CONS()`` macro.

Here, we have two constraints, which are just direct translations of what we have in the problem statement.

----

We now have a formal specification of our A+B problem. The next part is writing a test spec that specifies test cases which conform to the problem spec.

----

Test spec class
***************

.. sourcecode:: cpp

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        ...
    };

A **test spec** is a class that inherits ``tcframe::BaseTestSpec<T>``, where ``T`` is the problem spec class. It must be called ``TestSpec``.

This is where we actually write the test case definitions.

Test case definitions
*********************

.. sourcecode:: cpp

    void SampleTestCase1() {
        Input({
            "4 6"
        });
        Output({
            "10"
        });
    }

    void TestCases() {
        CASE(A = 1, B = 1);
        CASE(A = 1000, B = 1000);
        CASE(A = 42, B = 100);
        CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
    }

Here, we finally defined the test cases (yeay!). For the purpose of this guide, we defined four test cases: 3 hand-made and 1 randomized. We also defined one sample test case that match with the one in the actual problem statement.

In **tcframe**, sample test cases, if any, are defined in the ``SampleTestCaseX()`` methods, where ``X`` is the sample test case number. Each sample test case is defined as line-by-line verbatim strings in the ``Input()`` and ``Output()`` methods. Sample test cases must conform to the input format, or **tcframe** will complain.

Test cases are defined in the ``TestCases()`` method. Each test case is defined by listing input variable assignments the ``CASE()`` macro, separated by commas. Here, we just defined a min case, max case, random hand-made case, and a randomized case. The last one is achieved using ``tcframe::rnd``, a simple random number generator provided by **tcframe**.

.. note::

    Yes, you can access the input variables directly inside the test spec, even though they are declared in the problem spec class!

----

We've covered each component of our problem package in more details. Next, let's play around with our runner program.

----

Trying out invalid test cases
-----------------------------

What happens when we specify invalid test cases? Let's just try. Add this test case to our test spec:

.. sourcecode:: cpp

    CASE(A = 0, B = 1);

and this sample test case:

.. sourcecode:: cpp

    void SampleTestCase2() {
        Input({
            "1",
            "2"
        });
        Output({
            "3"
        });
    }

Recompile (by running ``tcframe build``) and rerun the runner program. You should now get the following output instead:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      sum_sample_1: OK
      sum_sample_2: FAILED
        Reasons:
        * Expected: <space> after variable `A`

    [ OFFICIAL TEST CASES ]
      sum_1: OK
      sum_2: OK
      sum_3: OK
      sum_4: OK
      sum_5: FAILED
        Description: A = 0, B = 1
        Reasons:
        * Does not satisfy constraints, on:
          - 1 <= A && A <= 1000

    Generation finished. Some test cases FAILED.

Sweet! If we ever have invalid test cases, **tcframe** will tell us in human-readable message.

Remove the invalid test cases and move on to the next section.

----

Local grading
-------------

When preparing a problem, it's ideal if we have at least another solution as an alternative/secondary solution. **tcframe** lets you "grade" another solution using the main solution as the reference. The time and memory limits from `GradingConfig()` as explained previously will be taken into consideration.

First, fix our spec file and re-run it to get back the correct test cases (important!). Then, write an alternate solution that deliberately behaves incorrectly on some test cases. Write the following as ``solution_alt.cpp``:

.. sourcecode:: cpp

    #include <iostream>
    using namespace std;

    int main() {
        int A, B;
        cin >> A >> B;

        if (A == 1) {
            cout << 3 << endl;
        } else if (A == 1000) {
            while (true);
        } else if (A == 42) {
            return 1 / 0;
        } else {
            cout << (A + B) << endl;
        }
    }

Compile the solution into ``solution_alt`` executable, and then run the following command:

::

    ./runner grade --solution=./solution_alt

The above command tells **tcframe** to run the specified alternate solution command against the output files previously produced by the main solution.

You should get the following output:

::

    Grading...

    [ SAMPLE TEST CASES ]
      sum_sample_1: Accepted

    [ OFFICIAL TEST CASES ]
      sum_1: Wrong Answer
        * Diff:
    (expected) [line 01]    2
    (received) [line 01]    3

      sum_2: Time Limit Exceeded
      sum_3: Runtime Error
        * Execution of submission failed:
          - Floating point exception: 8
      sum_4: Accepted

    [ RESULT ]
      Time Limit Exceeded

We get a detailed verdict of each test case. Nice, isn't it? The final result here is **Time Limit Exceeded**, which is the "worst" verdict among all test case verdicts.

----

We've covered the basics of **tcframe**. At this point, continue reading :ref:`Topic Guides <topic-guides>` for more in-depth explanation of **tcframe** features.

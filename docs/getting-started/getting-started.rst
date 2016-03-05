.. _getting-started:

Getting Started
===============

Let's write our first generator! We are going to write a test cases generator for the following infamous "A+B" problem:

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
| :code:`4 6`
|
| **Sample Output**
| :code:`10`
|
| **Constraints**
| - 1 ≤ A ≤ 1,000
| - 1 ≤ B ≤ 1,000
|

.. note::

    This starter guide will just demonstrate the basic features of **tcframe**. For more advanced features, please consult the **Topic Guides** afterwards.

----

Installation
------------

Firstly, we must get **tcframe** on our system. Fortunately, **tcframe** consists of just C++ header files: we don't actually need to "install" anything; we just have to :code:`#include` the header files in our program and we are ready to go.

Download the latest **tcframe** here: https://github.com/fushar/tcframe/releases/download/v0.6.0/tcframe_0.6.0.zip.

Extract the zip file somewhere on your system. For example, extract it to :code:`~/tcframe`.

----

Preparing working directory
---------------------------

Create a directory called :code:`aplusb` somewhere on your system; for example in your home (:code:`~/aplusb`). This will be our working directory for A+B problem. Store all the files you will be creating on this guide (solution and runner) in this directory.

----

Writing solution program
------------------------

In order to be able to generate the test case output files, a reference solution must be available. For the purpose of this guide, call the solution file :code:`solution.cpp`. Example solution:

.. sourcecode:: cpp

    #include <iostream>
    using namespace std;

    int main() {
        int A, B;
        cin >> A >> B;
        cout << (A + B) << endl;
    }

Then, compile it to :code:`solution` executable, for example:

::

    g++ -o solution solution.cpp

----

Writing runner program
----------------------

Now, we are ready to write the generator. In **tcframe** terms, the generator program is actually called a **runner** program, because it contains not only the generator but also the problem specification itself. Don't worry -- this will be clearer soon.

Create a C++ source file called :code:`runner.cpp`. Copy-paste the following code to the file:

.. sourcecode:: cpp

    #include <tcframe/runner.hpp>
    using namespace tcframe;

    class Problem : public BaseProblem {
    protected:
        int A, B;
        int sum;

        void Config() {
            setSlug("aplusb");
            setTimeLimit(1);
            setMemoryLimit(64);
        }

        void InputFormat() {
            LINE(A, B);
        }

        void OutputFormat() {
            LINE(sum);
        }

        void Constraints() {
            CONS(1 <= A && A <= 1000);
            CONS(1 <= B && B <= 1000);
        }
    };

    class Generator : public BaseGenerator<Problem> {
    protected:
        void Config() {
            setTestCasesDir("tc");
            setSolutionCommand("./solution");
        }

        void SampleTestCases() {
            SAMPLE_CASE({
                "4 6"
            });
        }

        void TestCases() {
            CASE(A = 1, B = 1);
            CASE(A = 1000, B = 1000);
            CASE(A = 42, B = 100);
            CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
        }
    };

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

We will explain this runner program later -- keep going!

Next, we will compile this runner program against **tcframe** headers. To do that, we need to add :code:`-I [/path/to/tcframe]/include` and :code:`-std=c++11` as compile options to **g++**. For example:

::

    g++ -I ~/tcframe/include -std=c++11 -o runner runner.cpp

Make sure that it compiles before continuing this getting started guide!

Finally, run the runner:

::

    ./runner

If everything is OK, you should get the following output:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      aplusb_sample_1: OK

    [ OFFICIAL TEST CASES ]
      aplusb_1: OK
      aplusb_2: OK
      aplusb_3: OK
      aplusb_4: OK

Congratulations, you have just written a runner program using **tcframe** framework! Check out your :code:`aplusb/tc` directory -- it will contain the generated test case files.

----

Inspecting runner program
-------------------------

We will now examine each component of the runner program in more details.

tcframe header
**************

.. sourcecode:: cpp

    #include <tcframe/runner.hpp>
    using namespace tcframe;

The :code:`tcframe/runner.hpp` is the main **tcframe**'s header file for runner programs. Each component of **tcframe** resides in the :code:`tcframe` namespace, just like the STL functions that reside in the :code:`std` namespace. By importing the namespace, we don't have to explicitly prefix each class/object we want to use with :code:`tcframe::`.

Problem specification class
***************************

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:
        ...
    };

A **problem specification** class is where we define the I/O format and constraints of our problem. This class must inherit :code:`tcframe::BaseProblem`. We just chose :code:`Problem` as the class name for simplicity.

All required members of this class must go in the **protected** section.

Problem configuration
*********************

.. sourcecode:: cpp

    void Config() {
        setSlug("aplusb");
        setTimeLimit(1);
        setMemoryLimit(64);
    }

What's going on here? We just specified several properties of our problem, that can be done in the :code:`Config()` method. :code:`setTimeLimit()` and :code:`setMemoryLimit()` should be self-explanatory. :code:`setSlug()` sets, well, the **slug**. A slug is a simple name/codename/identifier for the problem. The produced test cases will have the slug as the prefix of each test case file. We picked :code:`aplusb` for this particular problem.

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

Here, we declared two integers :code:`A` and :code:`B` as **input variables**, and an integer :code:`sum` as an **output variable**. :code:`InputFormat()` and :code:`OutputFormat()` methods specify the input/output formats in terms of the input/output variables. The :code:`LINE()` macro here specifies a line consisting of space-separated values of the given arguments.

Constraints
***********

.. sourcecode:: cpp

    void Constraints() {
        CONS(1 <= A && A <= 1000);
        CONS(1 <= B && B <= 1000);
    }

The last part of a problem specification is **constraints** specification.

A constraint must depend on input variables *only*. Each constraint can be specified as a boolean predicate inside the :code:`CONS()` macro.

Here, we have two constraints, which are just direct translations of what we have in the problem statement.

----

We now have a formal specification of our A+B problem. The next part is writing a generator that produces test cases which conform to that problem specification.

----

Generator specification class
*****************************

.. sourcecode:: cpp

    class Generator : public BaseGenerator<Problem> {
    protected:
        ...
    };

A **generator specification** is a class that inherits :code:`tcframe::BaseGenerator<T>`, where :code:`T` is the problem specification class. As usual, the name :code:`Generator` is just for simplicity -- it can be anything else.

This is where we actually write the test case definitions.

Generator configuration
***********************

.. sourcecode:: cpp

    void Config() {
        setTestCasesDir("tc");
        setSolutionCommand("./solution");
    }


Similar to the problem specification, we can set some properties of the generator with :code:`Config()` method.

Here, we tell **tcframe** to put all generated test case files in :code:`tc/` directory (relative to the current directory), and to use :code:`./solution` command to generate the output of each input file.

Test case definitions
*********************

.. sourcecode:: cpp

    void SampleTestCases() {
        SAMPLE_CASE({
            "4 6"
        });
    }

    void TestCases() {
        CASE(A = 1, B = 1);
        CASE(A = 1000, B = 1000);
        CASE(A = 42, B = 100);
        CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
    }

Here, we finally defined the test cases (yeay!). For the purpose of this guide, we defined four test cases: 3 hand-made and 1 randomized. We also defined one sample test case that match with the one in the actual problem statement.

In **tcframe**, sample test cases, if any, are defined in the :code:`SampleTestCases()` method. Each sample test case is defined as line-by-line verbatim strings in the :code:`SAMPLE_CASE()` macro. Sample test cases must conform to the input format, or **tcframe** will complain.

Test cases are defined in the :code:`TestCases()` method. Each test case is defined by listing input variable assignments the :code:`CASE()` macro, separated by commas. Here, we just defined a min case, max case, random hand-made case, and a randomized case. The last one is achieved using :code:`tcframe::rnd`, a simple random number generator provided by **tcframe**.

.. note::

    Yes, you can access the input variables directly inside the generator, even though they are declared in the problem specification class!

----

That's it for generator specification class. Problem and generator specification classes will be then managed by our :code:`main()` function.

----

Main function
*************

.. sourcecode:: cpp

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

The specification classes are ultimately instantiated here. We constructed runner object of our problem, set the generator, and then ran it.

.. note::

    In most cases, you would want to just copy-paste this :code:`main()` function to your runner programs -- you don't have to modify it at all.

----

We've covered each component of a our runner program in more details. Next, let's play around with our runner program.

----

Trying out invalid test cases
-----------------------------

What happens when we specify invalid test cases? Let's just try. Add this test case to our generator:

.. sourcecode:: cpp

    CASE(A = 0, B = 1);

and this sample test case:

.. sourcecode:: cpp

    SAMPLE_CASE({
        "1",
        "2"
    });

Recompile and rerun the runner program. You should now get the following output instead:

::

    Generating test cases...

    [ SAMPLE TEST CASES ]
      aplusb_sample_1: OK
      aplusb_sample_2: FAILED
        Reasons:
        * Expected: <space> after variable `A`

    [ OFFICIAL TEST CASES ]
      aplusb_1: OK
      aplusb_2: OK
      aplusb_3: OK
      aplusb_4: OK
      aplusb_5: FAILED
        Description: A = 0, B = 1
        Reasons:
        * Does not satisfy constraints, on:
          - 1 <= A && A <= 1000

Sweet! If we ever have invalid test cases, **tcframe** will tell us in human-readable message.

Remove the invalid test cases and move on to the next section.

----

Simulating submission
---------------------

When preparing a problem, it's ideal if we have at least another solution as an alternative/secondary solution. **tcframe** lets you "submit" another solution using the main solution as the reference.

First, fix our runner and re-run it to get back the correct test cases (important!). Then, write an alternate solution that deliberately behaves incorrectly on some test cases. Write the following as :code:`solution_alt.cpp`:

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

Compile the solution into :code:`solution_alt` executable, and then run the following command:

::

    ./runner submit --solution-command=./solution_alt

The above command tells **tcframe** to run the specified alternate solution command against the output files previously produced by the main solution.

You should get the following output:

::

    Submitting...

    [ SAMPLE TEST CASES ]
      aplusb_sample_1: Accepted

    [ OFFICIAL TEST CASES ]
      aplusb_1: Wrong Answer
        * Diff:
    (expected) [line 01]    2
    (received) [line 01]    3

      aplusb_2: Time Limit Exceeded
      aplusb_3: Runtime Error
        * Execution of submission failed:
          - Floating point exception: 8
      aplusb_4: Accepted

    [ RESULT ]
    Time Limit Exceeded

We get a detailed verdict of each test case. Nice, isn't it? The final result here is **Time Limit Exceeded**, which is the "worst" verdict among all test case verdicts.

----

We've covered the basics of **tcframe**. At this point, continue reading :ref:`Topic Guides <topic-guides>` for more in-depth explanation of **tcframe** features.

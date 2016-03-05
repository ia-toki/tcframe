.. _runner-program:

Runner program
==============

The core activity when preparing test cases with **tcframe** is writing runner programs. A runner program, along with a reference solution program, completely define the test cases of a single problem. A runner program is compilable into a single executable, which generates the test cases when run.

To write a runner program, create a C++ source file, and include the following header:

.. sourcecode:: cpp

    #include <tcframe/runner.hpp>

Every component of **tcframe** falls under :code:`tcframe` namespace, so you might want to import it for convenience:

.. sourcecode:: cpp

    using namespace tcframe;

----

Components of a runner program
------------------------------

Each runner program consists of the following components.

Problem specification
    Where you specify the problem's time/memory limits, I/O variables, I/O formats, constraints, etc. It is a very formal specification of what are stated in the problem description.

Generator specification
    Where you define the sample and official test cases, which solution to use, where to store the generated test cases, etc. It is tied to a particular problem specification: each test case must conform to the problem constraints, etc.

----

Writing main() function
-----------------------

To be able to be compiled into an executable, a runner program must have a :code:`main()` function. Currently, there is only one way to write the :code:`main()` function, as follows.

.. sourcecode:: cpp

    int main(int argc, char* argv[]) {
        Runner<Problem> runner(argc, argv);

        runner.setGenerator(new Generator());
        return runner.run();
    }

In most cases, you would want to just copy and paste the above snippet as your :code:`main()` function.

----

Compiling runner program
------------------------

You must have :code:`g++` at least version 4.7 to compile a runner program. It must be compiled with the following additional options:

- :code:`-I <tcframe-location>/include`, where :code:`tcframe-location` is where you downloaded and extracted **tcframe**.
- :code:`-std=c++11`, since **tcframe** relies on C++11 features.

For example:

::

    g++ -I ~/tcframe/include -std=c++11 -o runner runner.cpp

----

Running runner program
----------------------

A runner program is an ordinary executable program. It can be run by executing it:

::

    ./runner

The above execution will produce the actual test case files.

----

Notes
-----

- A runner consists of only problem and generator specifications. We are planning to support other components, for example, checker and inline solution specifications.
- A problem specification and the corresponding generator specification must be in the same file. We might support separating the specifications into two files.

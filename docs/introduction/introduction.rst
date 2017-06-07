.. _introduction:

Introduction
============

**tcframe** is a C++ framework for generating test cases of competitive programming problems. This framework helps problem writers prepare test cases in a structured manner and ensures that the generated test cases are valid according to the specified constraints.

Example high-level usage:

#. Specify input/output variables.

   .. sourcecode:: cpp

       int A, B;
       int sum;

#. Specify input/output formats, using a rich set of format macros.

   .. sourcecode:: cpp

       void InputFormat() {
           LINE(A, B); // A line containing space-separated A and B
       }
       void OutputFormat() {
           LINE(sum);
       }

#. Specify the grading configuration.

   .. sourcecode:: cpp

       void GradingConfig() {
           TimeLimit(2);
           MemoryLimit(64);
       }

#. Specify the constraints. Subtasks are supported.

   .. sourcecode:: cpp

       void Constraints() {
           CONS(1 <= A && A <= 1000);
           CONS(1 <= B && B <= 1000);
       }

#. Specify the sample test cases.

   .. sourcecode:: cpp

       void SampleTestCase1() {
           Input({
               "2 8"
           });
           Output({
               "10"
           });
       }
       void SampleTestCase2() {
           Input({
               "42 100"
           });
           Output({
               "142"
           });
       }

#. Specify the official test cases. Simple random number generator is available.

   .. sourcecode:: cpp

       void TestCases() {
           CASE(A = 1, B = 1);
           CASE(A = 77, B = 99);
           CASE(A = rnd.nextInt(1, 1000), B = rnd.nextInt(1, 1000));
       }

#. Write and compile the official solution to this problem, using any programming language you wish. Of course, it is the infamous A+B problem.

   .. sourcecode:: cpp

       #include <iostream>
       using namespace std;

       int main() {
           int A, B;
           cin >> A >> B;
           cout << (A + B) << endl;
       }

#. Run the generator. Actual test cases (``.in`` and ``.out`` files) will be generated. Profit!

#. If you ever specified an invalid test case, such as ``CASE(A = 0, B = 1)``, you will get a nice error message:

   .. sourcecode:: bash

       sum_4: FAILED
         Description: A = 0, B = 1
         Reasons:
         * Does not satisfy constraints, on:
           - 1 <= A && A <= 1000

----

Features
--------

**tcframe** supports:

- Batch and interactive problems.
- ICPC-style problems and IOI-style problems with subtasks and points.
- Multiple test cases per file.
- Local grading against the generated test cases, with time and memory limits.
- Simple random number generation helper.

----

Requirements
------------

**tcframe** requires:

- Linux/OS X. Windows is not supported.
- GCC ≥ 4.8. **tcframe** relies heavily on C++11 features.

----

Motivations
-----------

**Why do we need test case generators?**

- Writing test cases manually is error-prone and time-consuming.
- To enable distributing the test cases as a single, small generator file. No need to send 20 MB of ``testcases.zip`` over email anymore.
- During problem development, constraints often change. Using a generator, we can easily amend the constraints and rerun the generator when needed.

**Why do we need a framework for that?**

- Not everyone knows how to write a good test cases generator.
- To avoid writing repetitive and boring tasks. For example: creating test case files with correct suffixes (``foo_1.in``, ``foo_1.out``), running the official solution against the test case input files, etc.
- To have a consistent format for generators, so that problem writers in a contest can better collaborate in writing test case generators.

----

Credits
-------


**tcframe** is based on a paper submitted to IOI conference in 2015: `Introducing tcframe: A Simple and Robust Test Cases Generation Framework <http://ioinformatics.org/oi/files/volume9.pdf#page=59>`_, written by **Ashar Fuadi**.

**tcframe** was mainly inspired from `testlib <https://github.com/MikeMirzayanov/testlib>`_, written by **Mike Mirzayanov** et al.

----

License
-------

**tcframe** is released under MIT license.

Source code can be found on `GitHub <https://github.com/tcframe/tcframe>`_.

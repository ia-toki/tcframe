.. _test-groups:

Test groups
===========

A **test group** is a set of test cases that are assigned/included to the same set of subtasks. :ref:`If your problem has subtasks <subtasks>`, then instead of writing :code:`TestCases()` method, you will be writing :code:`TestGroupX()` methods instead, where :code:`X` is the test group number.

For the whole test cases to be strong, a test case should be assigned in multiple subtasks if possible. In other words, several subtasks may share some test cases. In **tcframe**, a test case **must** be assigned to a subtask if (and only if) it satisfies the constraints of that subtask.

----

Designing test groups
---------------------

In this example, we will use this subtasking scheme:

| **Subtask 1**
| - N = 1
| - 1 ≤ K ≤ 100
|
| **Subtask 2**
| - 1 ≤ N ≤ 100
| - K = 1
|
| **Subtask 3**
| - 1 ≤ N ≤ 100
| - 1 ≤ K ≤ 100
|

Our advice on designing test groups is as follows.

First, create a Venn diagram denoting the valid test cases for all subtasks. For this problem, the diagram will be like this:

.. image:: venn-diagram.png

In order to have a strong set of test cases, we should create a test group for each **closed region** in the Venn diagram. In this case, we will have four test groups as follows:

- Test group 1: consists of only one test case **N** = **K** = 1. Assign it to subtasks {1, 2, 3}.
- Test group 2: generate test cases that satisfy **N** = 1; 2 ≤ **K** ≤ 100. Assign them to subtasks {1, 3}.
- Test group 3: generate test cases that satisfy 2 ≤ **N** ≤ 100; **K** = 1. Assign them to subtasks {2, 3}.
- Test group 4: generate test cases that satisfy 2 ≤ **N**, **K** ≤ 100. Assign them to subtasks {3}.

----

Test group definitions
----------------------

To define test groups, write each of the methods :code:`TestGroupX()`, where :code:`X` is a positive integer denoting the test group number, starting from 1. Then, call the :code:`assignToSubtasks(S)` method as the first statement, where :code:`S` is a list of subtask numbers. The remaining bodies of test group methods are test case definitions. For the above example:

.. sourcecode:: cpp

    void TestGroup1() {
        assignToSubtasks({1, 2, 3});
        // CASE() calls follow
    }
    void TestGroup2() {
        assignToSubtasks({1, 3});
        // CASE() calls follow
    }
    void TestGroup3() {
        assignToSubtasks({2, 3});
        // CASE() calls follow
    }
    void TestGroup4() {
        assignToSubtasks({3});
        // CASE() calls follow
    }

.. note::

    As of this version, you can define up to 10 test groups: **TestGroup1()** .. **TestGroup10()**.

Each test case definition will be realized as a pair of input/output files, with the following filenames: :code:`<slug>_<group-number>_<case-number>.in` and :code:`<slug>_<group-number>_<case-number>.out`. For example, :code:`aplusb_2_3.in`, :code:`aplusb_2_3.out`.

----

Notes
-----

Although a test group may be assigned to several subtasks, **tcframe** will produce the test case files according to their test group and test case number as defined above. It is up to the implementation of the grader on how to choose which test cases to grade for a subtask. For example, if your grader only accepts a set of test cases for each subtask, you can duplicate each test case for every subtask it is assigned to.

For **tcframe**'s :ref:`submission simulation <submission>` feature, each test case will be executed only once. The result is shared for every subtask it is assigned to.
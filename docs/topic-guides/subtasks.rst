.. _subtasks:

Subtasks
========

The concept of **subtasks** was first formally introduced in IOI 2010. The idea is basically to split a problem into two or more smaller problems with gradually increasing difficulty, and assign them different points.

For example:

| **Subtask 1 (20 points)**
| - N = 1
| - 1 ≤ K ≤ 100
|
| **Subtask 2 (20 points)**
| - 1 ≤ N ≤ 100
| - K = 1
|
| **Subtask 3 (60 points)**
| - 1 ≤ N ≤ 100
| - 1 ≤ K ≤ 100
|

In this type type of problem, you won't be writing a :code:`Constraints()` method. Instead, you will be writing :code:`SubtaskX()` methods, where :code:`X` is the subtask number.

.. note::

    As of this version, you can define up to 25 subtasks: **Subtask1()** .. **Subtask25()**.

----

Subtask definitions
-------------------

A subtask is basically just a set of constraints. It can be specified by the method :code:`SubtaskX()`, where :code:`X` is the subtask number. Inside the method, the constraint specifications are given, similar to what you usually do in the :code:`Constraints()` method.

Thus, the above example can be implemented as:

.. sourcecode:: cpp

    void Subtask1() {
        CONS(N == 1);
        CONS(1 <= K && K <= 100);
    }

    void Subtask2() {
        CONS(1 <= N && N <= 100);
        CONS(K == 1);
    }

    void Subtask3() {
        CONS(1 <= N && N <= 100);
        CONS(1 <= K && K <= 100);
    }

----

Test groups
-----------

If your problem has subtasks, you will be writing test groups, not test cases. Refer to this :ref:`chapter <test-groups>` on how to do that.

----

Notes
-----

Currently, **tcframe** does not provide a way to specify subtask points. It is up to the grader to implement the scoring.
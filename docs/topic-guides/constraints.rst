.. _constraints:

Constraints
===========

**Constraints** are conditions that must be satisfied by the input variables. The conditions will be verified on each test case. If any of the constraints is not satisfied, then the generation is considered to fail on that test case. There will be a nice error message explaining which constraints are not satisfied.

The truth value of a constraint must depend **only** on the values of the input variables in order for it to be a semantically correct constraint.

Constraints can be specified in the :code:`Constraints()` method of the problem specification class.

.. sourcecode:: cpp

    void Constraints() {
        // list of constraint definitions
    }

----

Constraint definitions
----------------------

Constraints are specified as a list of **constraint definitions**. Each constraint definition is a boolean predicate of some property of the input variables. The predicate is passed as an argument to the :code:`CONS()` macro. The macros are then called one-by-one as method calls:

.. sourcecode:: cpp

    void Constraints() {
        CONS(<predicate 1>);
        CONS(<predicate 2>);
        ...
    }

The constraint definition can be pulled directly from the constraints section in the actual problem description. For example: "1 ≤ **A** ≤ 1,000" can be specified by :code:`CONS(1 <= A && A <= 100)`. "1 ≤ **A**\ , **B** ≤ 1,000" translates into two specifications: :code:`CONS(1 <= A && A <= 1000)` and :code:`CONS(1 <= B && B <= 1000)`. "1 ≤ **A** ≤ **B** ≤ 1,000" translates into :code:`CONS(1 <= A && A <= B && B <= 1000)`.

How about more complex predicates, such as "1 ≤ **A**\ [i] ≤ 1,000"? You can write a private method for this purpose. For example, it can be translated into :code:`CONS(eachElementBetween(A, 1, 1000))` and a private method as follows:

.. sourcecode:: cpp

    bool eachElementBetween(const vector<int>& X, int lo, int hi) {
        for (int x : X) {
            if (x < lo || x > hi) {
                return false;
            }
        }
        return true;
    }

This also applies to even more complex predicates, such as "It is guaranteed that the given graph is a tree". This can be translated to :code:`CONS(graphIsTree())` and define the appropriate private boolean method.

----

Notes
-----

It is tedious to write :code:`eachElementBetween()` predicate over and over again as it is very common in problems. We are working on providing such helper methods as core part of **tcframe** in the next versions.

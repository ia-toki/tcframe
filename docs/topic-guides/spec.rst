.. _spec:

Spec and runner
===============

The core activity when preparing test cases using **tcframe** is writing spec files. A spec file, along with a reference solution program, completely define the test cases of a single problem. A spec file is compilable into a single executable called **runner program**, which generates the test cases when executed.

To write a spec file, create a C++ source file called ``spec.cpp`` in the problem package directory, and include the following header:

.. sourcecode:: cpp

    #include <tcframe/spec.hpp>

Every component of **tcframe** falls under ``tcframe`` namespace, so you might want to import it for convenience:

.. sourcecode:: cpp

    using namespace tcframe;

In this file, you will be writing two classes: ``ProblemSpec`` and ``TestSpec``.

----

.. _problem-spec:

Problem spec
------------

A **problem spec** is a formal representation of a problem statement/description. It is the first step and the source of truth of the entire preparation of a problem. Once a problem spec is finalized, the corresponding test spec and a reference solution can then be developed.

In **tcframe**, a problem spec is implemented as a class called ``ProblemSpec`` that inherits ``tcframe::BaseProblemSpec``.

.. sourcecode:: cpp

    class ProblemSpec : public BaseProblemSpec {
    protected:
        ...
    };

All members of this class must go into the **protected** section, except for private helper methods.

:ref:`I/O variables <io-variables>`
    Member variables which compose the I/O formats.

:ref:`I/O formats <io-formats>`
    How I/O variables are arranged in actual test case files.

:ref:`Constraints <constraints>`
    Conditions that I/O variables must satisfy.

:ref:`Subtasks <subtasks>`
    Set of constraints in subproblems.

----

.. _test-spec:

Test spec
---------

A **test spec** defines the actual test cases for a particular problem spec. For problems with subtasks, it also defines how the test cases are distributed to the subtasks.

In **tcframe**, a problem spec is implemented as a class called ``TestSpec`` that inherits ``tcframe::BaseTestSpec<T>``, where ``T`` is the problem spec class (which should be ``ProblemSpec``).

.. sourcecode:: cpp

    class TestSpec : public BaseTestSpec<ProblemSpec> {
    protected:
        ...
    };

All members of this class must go in the **protected** section, except for private helper methods.

:ref:`Test cases <test-cases>`
    Particular set of values of input variables.

:ref:`Test groups <test-groups>`
    Set of test cases which conform to the same set of subtasks

----

Compiling spec file
-------------------

You must have **g++** at least version 4.8 to compile a spec file.

A spec file is compiled into a runner program using the following **tcframe** command:

::

    tcframe build

The above command will compile ``spec.cpp`` into an executable ``runner`` program in the problem package directory.

It is also possible to specify additional compilation flags, by setting the ``$TCFRAME_CXX_FLAGS`` environment variable. For example:

::

    export TCFRAME_CXX_FLAGS="-Wall -O2"

----

Runner program
--------------

A runner program is an ordinary executable program. By executing the runner program, the test cases will be generated. By default, the produced test cases will be output to ``tc`` directory inside problem package directory.

::

    ./runner

See the :ref:`API reference <api-ref_runner>` for more details on supported command-line options, such as specifying which solution to run for producing the output files.

A runner program can also be used for performing :ref:`local grading <grading>`.

.. _problem-package:

Problem package
===============

A problem package is the unit of work in **tcframe** that defines a problem and its test data. It is a directory that consists of all files related to the problem, particularly a spec file and one or more solution files. It is identified by a unique name called **slug**.

Slug
----

A slug can be considered as the codename of a problem. For example, if your problem name is "Counting Tree", the slug could be ``tree`` or ``counting-tree`` or whatever you like, as long as it consists of one or more characters ``a``-``z``, ``A``-``Z``, ``0``-``9``, and ``-``. The produced test case files will have the slug as the prefix, for example: ``tree_1.in``.

The slug will be taken from the name of the problem package's directory. For example, if your problem package directory is ``/home/fushar/my-contest/tree/``, then the slug would be ``tree``.

It is also possible to prepend the slug with some metadata string, separated by an underscore (``_``). For example, if ``tree`` if your third problem in your contest, you might want to call your problem package directory ``/home/fushar/my-contest/c_tree/``. In this case, the slug would be still ``tree``.

Components of a problem package
-------------------------------

:ref:`Spec file <spec>`
    The formal specification of the problem.

Solution files
    A reference solution and one or more alternate solutions to the problem.

:ref:`Evaluator helper files <styles_helpers>`
   Optional, e.g. custom scorer and communicator.

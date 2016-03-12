.. _problem-spec:

Problem specification
=====================

A **problem specification** is a formal representation of a problem statement/description. It is the first step and the source of truth of the entire preparation of a problem. Once a problem specification is finalized, the corresponding generator specification and a reference solution can then be written.

In **tcframe**, a problem specification is implemented as a class that inherits :code:`tcframe::BaseProblem`. The class name can be arbitrary, but usually there is no reason to name the class other than :code:`Problem`.

.. sourcecode:: cpp

    class Problem : public BaseProblem {
    protected:
        ...
    };

All members of this class must go in the **protected** section, except for private helper methods.

----

Configuring problem
-------------------

Several properties of the problem can be configured by overriding :code:`Config()` method. In the method, we can set a property by calling the appropriate method as follows.

Slug
    By calling :code:`setSlug(<slug>)`. A slug is a simple name, codename, or identifier for the problem. For example: :code:`aplusb`, :code:`wf-2016-a`, :code:`tree-paint`, etc. The resulting test case files will be prefixed by the slug, for example: :code:`aplusb_1.in`.

Time limit
    By calling :code:`setTimeLimit(<timeLimit>)`. Time limit is specified in seconds. Time limit will be enforced when simulating submissoin.

Memory limit
    By calling :code:`setMemoryLimit(<memoryLimit>)`. Memory limit is specified in megabytes. Similarly, it will be enforced when simulating submission.

Here is an example of a valid problem configuration:

.. sourcecode:: cpp

    void Config() {
        setSlug("rabbit-jump");
        setTimeLimit(2);
        setMemoryLimit(256);
    }

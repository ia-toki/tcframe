.. _generator-spec:

Generator specification
=======================

A **generator specification** defines the actual test cases for a particular problem specification. For problems with subtasks, it also defines how the test cases are distributed to the subtasks.

In **tcframe**, a generator specification is implemented as class that inherits :code:`tcframe::BaseGenerator<TProblem>`. Here, :code:`TProblem` is the problem specification class, which is usually :code:`Problem`. The class name can be arbitrary, but usually there is no reason to name the class other than :code:`Generator`.

.. sourcecode:: cpp

    class Generator : public BaseGenerator<Problem> {
    protected:
        ...
    };

All members of this class must go in the **protected** section, except for private helper methods.

----

Configuring generator
---------------------

Several properties of the problem can be configured by overriding :code:`Config()` method. In the method, we can set a property by calling the appropriate method as follows.

Test cases directory
    By calling :code:`setTestCasesDir(<directory>)`. This directory will contain the generated test cases. The directory is specified as a relative path to the runner. The default value is :code:`tc`.

Solution command
    By calling :code:`setSolutionCommand(<command>)`. The command for executing the reference solution. The default value is :code:`./solution`.

Here is an example of a valid problem configuration:

.. sourcecode:: cpp

    void Config() {
        setTestCasesDir("generated-tc");
        setSolutionCommand("java Solution");
    }

For simplicity, usually you don't have to specify this method at all and just accept/use the default values.

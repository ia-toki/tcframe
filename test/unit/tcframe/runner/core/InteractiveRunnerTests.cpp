#include "BaseRunnerTests.hpp"

namespace tcframe {

class InteractiveRunnerTests : public BaseRunnerTests {
protected:
    class InteractiveProblemSpec : public ProblemSpec {
    protected:
        void StyleConfig() {
            InteractiveEvaluator();
        }
    };

    class InteractiveWithRedundantNoOutputProblemSpec : public ProblemSpec {
    protected:
        void StyleConfig() {
            InteractiveEvaluator();
            NoOutput();
        }
    };

    class InteractiveTestSpec : public BaseTestSpec<InteractiveProblemSpec> {};
    class InteractiveWithRedundantNoOutputTestSpec :
            public BaseTestSpec<InteractiveWithRedundantNoOutputProblemSpec> {};

    Runner<InteractiveProblemSpec> runner = createRunner(new InteractiveTestSpec());
    Runner<InteractiveWithRedundantNoOutputProblemSpec> runnerWithRedundantNoOutput =
            createRunner(new InteractiveWithRedundantNoOutputTestSpec());
};

TEST_F(InteractiveRunnerTests, Run_Generation_NoOutput) {
    EXPECT_CALL(generator, generate(_, Property(&GenerationOptions::needsOutput, false)));
    runner.run(argc, argv);
}

TEST_F(InteractiveRunnerTests, Run_Generation_NoOutput_Redundant) {
    EXPECT_CALL(generator, generate(_, Property(&GenerationOptions::needsOutput, false)));
    runnerWithRedundantNoOutput.run(argc, argv);
}

TEST_F(InteractiveRunnerTests, Run_Generation_EvaluatorRegistry_Communicator_Default) {
    EXPECT_CALL(evaluatorRegistry, get(
            EvaluationStyle::INTERACTIVE,
            _,
            Truly(HelperKeyIs("communicator", RunnerDefaults::COMMUNICATOR_COMMAND))));

    runner.run(argc, argv);
}

TEST_F(InteractiveRunnerTests, Run_Generation_EvaluatorRegistry_Communicator_Args) {
    EXPECT_CALL(evaluatorRegistry, get(
            EvaluationStyle::INTERACTIVE,
            _,
            Truly(HelperKeyIs("communicator",  "\"java Communicator\""))));
    
    runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "--communicator=\"java Communicator\"",
            nullptr});
}

TEST_F(InteractiveRunnerTests, Run_Grading_EvaluatorRegistry_Communicator_Default) {
    EXPECT_CALL(evaluatorRegistry, get(
            EvaluationStyle::INTERACTIVE,
            _,
            Truly(HelperKeyIs("communicator", RunnerDefaults::COMMUNICATOR_COMMAND))));

    runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "grade",
            nullptr});
}

TEST_F(InteractiveRunnerTests, Run_Grading_EvaluatorRegistry_Communicator_Args) {
    EXPECT_CALL(evaluatorRegistry, get(
            EvaluationStyle::INTERACTIVE,
            _,
            Truly(HelperKeyIs("communicator", "\"java Communicator\""))));

    runner.run(3, new char*[4]{
            (char*) "./runner",
            (char*) "grade",
            (char*) "--communicator=\"java Communicator\"",
            nullptr});
}

}

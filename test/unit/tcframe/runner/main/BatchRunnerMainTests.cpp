#include "BaseRunnerMainTests.hpp"

namespace tcframe {

class BatchRunnerMainTests : public BaseRunnerMainTests {
protected:
    class BatchProblemSpec : public ProblemSpec {
    protected:
        void StyleConfig() {
            BatchEvaluator();
        }
    };

    class BatchWithCustomScorerProblemSpec : public ProblemSpec {
    protected:
        void StyleConfig() {
            BatchEvaluator();
            CustomScorer();
        }
    };

    class BatchTestSpec : public BaseTestSpec<BatchProblemSpec> {};
    class BatchWithCustomScorerTestSpec : public BaseTestSpec<BatchWithCustomScorerProblemSpec> {};

    RunnerMain<BatchProblemSpec> runner = createRunner(new BatchTestSpec());
    RunnerMain<BatchWithCustomScorerProblemSpec> runnerWithCustomScorer = createRunner(new BatchWithCustomScorerTestSpec());
};

TEST_F(BatchRunnerMainTests, Run_Generation_EvaluatorRegistry_NoCustomScorer) {
    EXPECT_CALL(evaluatorRegistry, get(EvaluationStyle::BATCH, _, Truly(HelperKeyIs("scorer", ""))));
    runner.run(argc, argv);
}

TEST_F(BatchRunnerMainTests, Run_Generation_EvaluatorRegistry_CustomScorer_Default) {
    EXPECT_CALL(evaluatorRegistry,
                get(EvaluationStyle::BATCH, _,  Truly(HelperKeyIs("scorer", string(RunnerDefaults::SCORER_COMMAND)))));

    runnerWithCustomScorer.run(argc, argv);
}

TEST_F(BatchRunnerMainTests, Run_Generation_EvaluatorRegistry_CustomScorer_Args) {
    EXPECT_CALL(evaluatorRegistry, get(EvaluationStyle::BATCH, _, Truly(HelperKeyIs("scorer", "\"java Scorer\""))));
    runnerWithCustomScorer.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "--scorer=\"java Scorer\"",
            nullptr});
}

TEST_F(BatchRunnerMainTests, Run_Grading_EvaluatorRegistry_NoCustomScorer) {
    EXPECT_CALL(evaluatorRegistry, get(EvaluationStyle::BATCH, _, Truly(HelperKeyIs("scorer", ""))));
    runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "grade",
            nullptr});
}

TEST_F(BatchRunnerMainTests, Run_Grading_EvaluatorRegistry_CustomScorer_Default) {
    EXPECT_CALL(evaluatorRegistry,
                get(EvaluationStyle::BATCH, _,  Truly(HelperKeyIs("scorer", string(RunnerDefaults::SCORER_COMMAND)))));

    runnerWithCustomScorer.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "grade",
            nullptr});
}

TEST_F(BatchRunnerMainTests, Run_Grading_EvaluatorRegistry_CustomScorer_Args) {
    EXPECT_CALL(evaluatorRegistry, get(EvaluationStyle::BATCH, _, Truly(HelperKeyIs("scorer", "\"java Scorer\""))));
    runnerWithCustomScorer.run(3, new char*[4]{
            (char*) "./runner",
            (char*) "./grade",
            (char*) "--scorer=\"java Scorer\"",
            nullptr});
}

}

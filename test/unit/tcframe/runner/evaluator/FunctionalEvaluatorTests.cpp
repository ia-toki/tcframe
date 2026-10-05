#include "gmock/gmock.h"
#include "../../mock.hpp"

#include <sstream>
#include <stdexcept>

#include "../os/MockOperatingSystem.hpp"
#include "../verdict/MockTestCaseVerdictParser.hpp"
#include "scorer/MockScorer.hpp"
#include "tcframe/runner/evaluator/FunctionalEvaluator.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::Property;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class FunctionalEvaluatorTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    MOCK(TestCaseVerdictParser) testCaseVerdictParser;
    MOCK(Scorer) scorer;

    EvaluationOptions options = EvaluationOptionsBuilder()
            .setSolutions(solutions())
            .build();

    FunctionalEvaluator evaluator = {&os, &testCaseVerdictParser, &scorer, "./manager"};

    const string buildCommand =
            "g++ -std=c++17 -O2 -I ./manager/cpp -o __tcframe_functional ./manager/cpp/grader.cpp dec.cpp enc.cpp";

    static SolutionMap solutions() {
        SolutionMap solutions;
        solutions.set("encoder", "enc.cpp");
        solutions.set("decoder", "dec.cpp");
        return solutions;
    }

    void SetUp() {
        ON_CALL(os, execute(_))
                .WillByDefault(Return(ExecutionResult()));
        ON_CALL(testCaseVerdictParser, parseExecutionResult(_))
                .WillByDefault(Return(optional<TestCaseVerdict>()));
    }
};

TEST_F(FunctionalEvaluatorTests, Generation_BuildsThenRunsGraderProgram) {
    auto buildRequest = ExecutionRequestBuilder().setCommand(buildCommand).build();
    auto runRequest = ExecutionRequestBuilder()
            .setCommand("./__tcframe_functional")
            .setInputFilename("dir/foo_1.in")
            .setOutputFilename("dir/foo_1.out")
            .build();
    {
        InSequence sequence;
        EXPECT_CALL(os, execute(buildRequest));
        EXPECT_CALL(os, execute(runRequest));
    }

    evaluator.generate("dir/foo_1.in", "dir/foo_1.out", options);
}

TEST_F(FunctionalEvaluatorTests, Generation_BuildsOnlyOncePerInstance) {
    EXPECT_CALL(os, execute(ExecutionRequestBuilder().setCommand(buildCommand).build())).Times(1);
    EXPECT_CALL(os, execute(Property(&ExecutionRequest::command, Eq("./__tcframe_functional"))))
            .Times(2);

    evaluator.generate("dir/foo_1.in", "dir/foo_1.out", options);
    evaluator.generate("dir/foo_2.in", "dir/foo_2.out", options);
}

TEST_F(FunctionalEvaluatorTests, Generation_CustomManagerDir) {
    FunctionalEvaluator custom = {&os, &testCaseVerdictParser, &scorer, "../shared/manager"};

    {
        InSequence sequence;
        EXPECT_CALL(os, execute(ExecutionRequestBuilder()
                .setCommand("g++ -std=c++17 -O2 -I ../shared/manager/cpp -o __tcframe_functional "
                            "../shared/manager/cpp/grader.cpp dec.cpp enc.cpp")
                .build()));
        EXPECT_CALL(os, execute(Property(&ExecutionRequest::command, Eq("./__tcframe_functional"))));
    }

    custom.generate("dir/foo_1.in", "dir/foo_1.out", options);
}

TEST_F(FunctionalEvaluatorTests, Generation_CustomBuildAndRunScripts) {
    istringstream existing("");
    const string dir = "../registry/evaluators/functional";
    FunctionalEvaluator pascal = {&os, &testCaseVerdictParser, &scorer, "./manager", dir, "pascal"};

    EXPECT_CALL(os, openForReading(dir + "/build_pascal")).WillRepeatedly(Return(&existing));
    EXPECT_CALL(os, openForReading(dir + "/run_pascal")).WillRepeatedly(Return(&existing));

    {
        InSequence sequence;
        EXPECT_CALL(os, execute(ExecutionRequestBuilder()
                .setCommand(dir + "/build_pascal dec.cpp enc.cpp ./manager")
                .build()));
        EXPECT_CALL(os, execute(ExecutionRequestBuilder()
                .setCommand(dir + "/run_pascal dir/foo_1.in ./manager")
                .setInputFilename("dir/foo_1.in")
                .setOutputFilename("dir/foo_1.out")
                .build()));
    }

    pascal.generate("dir/foo_1.in", "dir/foo_1.out", options);
}

TEST_F(FunctionalEvaluatorTests, Generation_NonCppWithoutBuildScriptIsError) {
    FunctionalEvaluator pascal = {&os, &testCaseVerdictParser, &scorer, "./manager", "", "pascal"};

    EXPECT_CALL(os, execute(_)).Times(0);

    EXPECT_THROW(pascal.generate("dir/foo_1.in", "dir/foo_1.out", options), runtime_error);
}

TEST_F(FunctionalEvaluatorTests, Generation_BuildFailureIsError) {
    ExecutionResult failed = ExecutionResultBuilder()
            .setExitCode(1)
            .setStandardError("enc.cpp:3: error: expected ';'\n")
            .build();
    EXPECT_CALL(os, execute(_)).WillOnce(Return(failed));

    EXPECT_THROW(evaluator.generate("dir/foo_1.in", "dir/foo_1.out", options), runtime_error);
}

TEST_F(FunctionalEvaluatorTests, Evaluation_ScoresRunOutput) {
    EXPECT_CALL(scorer, score("dir/foo_1.in", "dir/foo_1.out", Evaluator::EVALUATION_OUT_FILENAME))
            .WillOnce(Return(ScoringResult(TestCaseVerdict(Verdict::ac()), ExecutionResult())));

    EvaluationResult result = evaluator.evaluate("dir/foo_1.in", "dir/foo_1.out", options);

    EXPECT_THAT(result.verdict().verdict().code(), Eq(Verdict::ac().code()));
}

}

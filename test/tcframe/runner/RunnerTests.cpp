#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../generator/MockGenerator.hpp"
#include "../grader/MockGrader.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "MockRunnerLogger.hpp"
#include "MockRunnerLoggerFactory.hpp"
#include "tcframe/experimental/runner.hpp"

using ::testing::_;
using ::testing::Eq;
using ::testing::Ne;
using ::testing::Return;
using ::testing::Test;

namespace tcframe {

class RunnerTests : public Test {
public:
    static int T;

protected:
    class ProblemSpec : public BaseProblemSpec {
    protected:
        void InputFormat() {}
    };

    class ProblemSpecWithConfig : public ProblemSpec {
    protected:
        void MultipleTestCasesConfig() {
            Counter(T);
        }

        void GradingConfig() {
            TimeLimit(3);
            MemoryLimit(128);
        }
    };

    class TestSpec : public BaseTestSpec<ProblemSpec> {};

    class TestSpecWithConfig : public BaseTestSpec<ProblemSpecWithConfig> {};

    class BadTestSpec : public BaseTestSpec<ProblemSpec> {
    public:
        Spec buildSpec(const string&) {
            throw runtime_error("An error");
        }
    };

    int argc = 1;
    char** argv =  new char*[1]{(char*) "./slug"};

    LoggerEngine* loggerEngine = new SimpleLoggerEngine();

    MOCK(RunnerLogger) runnerLogger;
    MOCK(Generator) generator;
    MOCK(Grader) grader;

    MOCK(OperatingSystem) os;
    MOCK(RunnerLoggerFactory) runnerLoggerFactory;
    MOCK(GeneratorFactory) generatorFactory;
    MOCK(GraderFactory) graderFactory;

    Runner<ProblemSpec> runner = createRunner(new TestSpec());
    Runner<ProblemSpecWithConfig> runnerWithConfig = createRunner(new TestSpecWithConfig());

    void SetUp() {
        ON_CALL(runnerLoggerFactory, create(_)).WillByDefault(Return(&runnerLogger));
        ON_CALL(generatorFactory, create(_, _, _, _, _)).WillByDefault(Return(&generator));
        ON_CALL(graderFactory, create(_, _)).WillByDefault(Return(&grader));
        ON_CALL(os, execute(_)).WillByDefault(Return(
                ExecutionResult(ExecutionInfoBuilder().setExitCode(0).build(), nullptr, nullptr)));
    }

    template<typename TProblem>
    Runner<TProblem> createRunner(BaseTestSpec<TProblem>* testSpec) {
        return Runner<TProblem>(
                testSpec, loggerEngine, &os, &runnerLoggerFactory, &generatorFactory, &graderFactory);
    }
};

int RunnerTests::T;

TEST_F(RunnerTests, Run_ArgsParsing_Failed) {
    EXPECT_THAT(runner.run(2, new char*[2]{(char*) "./slug", (char*) "--blah"}), Ne(0));
}

TEST_F(RunnerTests, Run_Specification_Failed) {
    Runner<ProblemSpec> badRunner = createRunner(new BadTestSpec());

    EXPECT_CALL(generator, generate(_, _)).Times(0);
    EXPECT_CALL(runnerLogger, logSpecificationFailure(vector<string>{"An error"}));

    EXPECT_THAT(badRunner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_Successful) {
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(true));

    EXPECT_THAT(runner.run(argc, argv), Eq(0));
}

TEST_F(RunnerTests, Run_Generation_Failed) {
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(false));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_UseDefaultOptions) {
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder("slug")
            .setSeed(0)
            .setSolutionCommand("./solution")
            .setOutputDir("tc")
            .build()));

    runner.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseConfigOptions) {
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder("slug")
            .setMultipleTestCasesCounter(&T)
            .setSolutionCommand("./solution")
            .setOutputDir("tc")
            .build()));

    runnerWithConfig.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseArgsOptions) {
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder("slug")
            .setSeed(42)
            .setMultipleTestCasesCounter(&T)
            .setSolutionCommand("\"java Solution\"")
            .setOutputDir("testdata")
            .build()));

    runnerWithConfig.run(4, new char*[5]{
            (char*) "./slug",
            (char*) "--seed=42",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            nullptr});
}

TEST_F(RunnerTests, Run_Grading) {
    EXPECT_CALL(grader, grade(_, _, _));

    int exitStatus = runner.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "grade",
            nullptr});

    EXPECT_THAT(exitStatus, Eq(0));
}

TEST_F(RunnerTests, Run_Grading_UseDefaultOptions) {
    EXPECT_CALL(grader, grade(_, _, GraderConfigBuilder("slug")
            .setHasMultipleTestCases(false)
            .setTimeLimit(2)
            .setMemoryLimit(64)
            .setSolutionCommand("./solution")
            .setOutputDir("tc")
            .build()));

    runner.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "grade",
            nullptr});
}

TEST_F(RunnerTests, Run_Grading_UseConfigOptions) {
    EXPECT_CALL(grader, grade(_, _, GraderConfigBuilder("slug")
            .setHasMultipleTestCases(true)
            .setSolutionCommand("./solution")
            .setOutputDir("tc")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build()));

    runnerWithConfig.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "grade",
            nullptr});
}

TEST_F(RunnerTests, Run_Grading_UseArgsOptions) {
    EXPECT_CALL(grader, grade(_, _, GraderConfigBuilder("slug")
            .setHasMultipleTestCases(true)
            .setSolutionCommand("\"java Solution\"")
            .setOutputDir("testdata")
            .setTimeLimit(4)
            .setMemoryLimit(256)
            .build()));

    runnerWithConfig.run(6, new char*[7]{
            (char*) "./slug",
            (char*) "grade",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            (char*) "--time-limit=4",
            (char*) "--memory-limit=256",
            nullptr});
}

TEST_F(RunnerTests, Run_Grading_UseArgsOptions_NoLimits) {
    EXPECT_CALL(grader, grade(_, _, GraderConfigBuilder("slug")
            .setHasMultipleTestCases(true)
            .setSolutionCommand("\"java Solution\"")
            .setOutputDir("testdata")
            .build()));

    runnerWithConfig.run(6, new char*[7]{
            (char*) "./slug",
            (char*) "grade",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            (char*) "--no-time-limit",
            (char*) "--no-memory-limit",
            nullptr});
}

}

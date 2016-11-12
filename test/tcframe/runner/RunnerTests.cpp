#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../generator/MockGenerator.hpp"
#include "../os/MockOperatingSystem.hpp"
#include "../submitter/MockSubmitter.hpp"
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

        void Config() {
            MultipleTestCasesCount(T);
        }
    };

    class ProblemSpecWithConfig : public ProblemSpec {
    protected:
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
    MOCK(Submitter) submitter;

    MOCK(OperatingSystem) os;
    MOCK(RunnerLoggerFactory) runnerLoggerFactory;
    MOCK(GeneratorFactory) generatorFactory;
    MOCK(SubmitterFactory) submitterFactory;

    Runner<ProblemSpec> runner = createRunner(new TestSpec());

    void SetUp() {
        ON_CALL(runnerLoggerFactory, create(_)).WillByDefault(Return(&runnerLogger));
        ON_CALL(generatorFactory, create(_, _, _, _)).WillByDefault(Return(&generator));
        ON_CALL(submitterFactory, create(_, _)).WillByDefault(Return(&submitter));
        ON_CALL(os, execute(_)).WillByDefault(Return(
                ExecutionResult(ExecutionInfoBuilder().setExitCode(0).build(), nullptr, nullptr)));
    }

    template<typename TProblem>
    Runner<TProblem> createRunner(BaseTestSpec<TProblem>* testSpec) {
        return Runner<TProblem>(
                testSpec, loggerEngine, &os, &runnerLoggerFactory, &generatorFactory, &submitterFactory);
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

TEST_F(RunnerTests, Run_Generation_UseConfigOptions) {
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(0)
            .setSlug("slug")
            .setMultipleTestCasesCount(&T)
            .setSolutionCommand("./solution")
            .setOutputDir("tc")
            .build()));

    runner.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseArgsOptions) {
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(42)
            .setSlug("slug")
            .setMultipleTestCasesCount(&T)
            .setSolutionCommand("\"java Solution\"")
            .setOutputDir("testdata")
            .build()));

    runner.run(4, new char*[5]{
            (char*) "./slug",
            (char*) "--seed=42",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission) {
    EXPECT_CALL(submitter, submit(_, _, _));

    int exitStatus = runner.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "submit",
            nullptr});

    EXPECT_THAT(exitStatus, Eq(0));
}

TEST_F(RunnerTests, Run_Submission_UseDefaultOptions) {
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("slug")
            .setHasMultipleTestCasesCount(true)
            .setTimeLimit(2)
            .setMemoryLimit(64)
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .build()));

    runner.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "submit",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseConfigOptions) {
    Runner<ProblemSpecWithConfig> runnerWithConfig = createRunner(new TestSpecWithConfig());

    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("slug")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build()));

    runnerWithConfig.run(2, new char*[3]{
            (char*) "./slug",
            (char*) "submit",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseArgsOptions) {
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("slug")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .setTimeLimit(4)
            .setMemoryLimit(256)
            .build()));

    runner.run(6, new char*[7]{
            (char*) "./slug",
            (char*) "submit",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            (char*) "--time-limit=4",
            (char*) "--memory-limit=256",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseArgsOptions_NoLimits) {
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("slug")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .build()));

    runner.run(6, new char*[7]{
            (char*) "./slug",
            (char*) "submit",
            (char*) "--solution=\"java Solution\"",
            (char*) "--output=testdata",
            (char*) "--no-time-limit",
            (char*) "--no-memory-limit",
            nullptr});
}

}

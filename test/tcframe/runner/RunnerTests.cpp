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
    };

    class ProblemSpecWithConfig : public ProblemSpec {
    protected:
        void Config() {
            Slug("foo");
            MultipleTestCasesCount(T);
            TimeLimit(3);
            MemoryLimit(128);
        }
    };

    class TestSpec : public BaseTestSpec<ProblemSpec> {};

    class TestSpecWithConfig : public BaseTestSpec<ProblemSpecWithConfig> {};

    class BadTestSpec : public BaseTestSpec<ProblemSpec> {
    public:
        CoreSpec buildCoreSpec() {
            throw runtime_error("An error");
        }
    };

    int argc = 1;
    char** argv =  new char*[1]{(char*) "./runner"};

    LoggerEngine* loggerEngine = new SimpleLoggerEngine();

    Mock(RunnerLogger) logger;
    Mock(Generator) generator;
    Mock(Submitter) submitter;

    Mock(OperatingSystem) os;
    Mock(RunnerLoggerFactory) loggerFactory;
    Mock(GeneratorFactory) generatorFactory;
    Mock(SubmitterFactory) submitterFactory;

    void SetUp() {
        ON_CALL(loggerFactory, create(_)).WillByDefault(Return(&logger));
        ON_CALL(generatorFactory, create(_, _, _, _)).WillByDefault(Return(&generator));
        ON_CALL(submitterFactory, create(_, _)).WillByDefault(Return(&submitter));
    }
};

int RunnerTests::T;

TEST_F(RunnerTests, Run_ArgsParsing_Failed) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_THAT(runner.run(2, new char*[2]{(char*) "./runner", (char*) "--blah"}), Ne(0));
}

TEST_F(RunnerTests, Run_Specification_Failed) {
    Runner<ProblemSpec> runner(new BadTestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(generator, generate(_, _)).Times(0);
    EXPECT_CALL(logger, logSpecificationFailure(vector<string>{"An error"}));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_Successful) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(true));

    EXPECT_THAT(runner.run(argc, argv), Eq(0));
}

TEST_F(RunnerTests, Run_Generation_Failed) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(false));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_UseDefaultOptions) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(0)
            .setSlug("problem")
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .build()));

    runner.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseSuppliedOptions) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(0)
            .setSlug("foo")
            .setMultipleTestCasesCount(&T)
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .build()));

    runner.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseArgsOptions) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(42)
            .setSlug("foo")
            .setMultipleTestCasesCount(&T)
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .build()));

    runner.run(4, new char*[5]{
            (char*) "./runner",
            (char*) "--seed=42",
            (char*) "--solution=\"java Solution\"",
            (char*) "--tc-dir=testdata",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(submitter, submit(_, _, _));

    int exitStatus = runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "submit",
            nullptr});

    EXPECT_THAT(exitStatus, Eq(0));
}

TEST_F(RunnerTests, Run_Submission_UseDefaultOptions) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("problem")
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .build()));

    runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "submit",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseSuppliedOptions) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("foo")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .setTimeLimit(3)
            .setMemoryLimit(128)
            .build()));

    runner.run(2, new char*[3]{
            (char*) "./runner",
            (char*) "submit",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseArgsOptions) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("foo")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .setTimeLimit(4)
            .setMemoryLimit(256)
            .build()));

    runner.run(6, new char*[7]{
            (char*) "./runner",
            (char*) "submit",
            (char*) "--solution=\"java Solution\"",
            (char*) "--tc-dir=testdata",
            (char*) "--time-limit=4",
            (char*) "--memory-limit=256",
            nullptr});
}

TEST_F(RunnerTests, Run_Submission_UseArgsOptions_NoLimits) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory, &submitterFactory);
    EXPECT_CALL(submitter, submit(_, _, SubmitterConfigBuilder()
            .setSlug("foo")
            .setHasMultipleTestCasesCount(true)
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .build()));

    runner.run(6, new char*[7]{
            (char*) "./runner",
            (char*) "submit",
            (char*) "--solution=\"java Solution\"",
            (char*) "--tc-dir=testdata",
            (char*) "--no-time-limit",
            (char*) "--no-memory-limit",
            nullptr});
}

}

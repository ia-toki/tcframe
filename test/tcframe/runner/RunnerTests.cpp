#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../generator/MockTestSuiteGenerator.hpp"
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
protected:
    class ProblemSpec : public BaseProblemSpec {
    protected:
        void InputFormat() {}
    };

    class ProblemSpecWithConfig : public ProblemSpec {
    protected:
        void Config() {
            setSlug("foo");
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

    class DeprecatedProblem : public BaseProblem {
    protected:
        void InputFormat() {}
    };

    class DeprecatedGenerator : public BaseGenerator<DeprecatedProblem> {};

    int argc = 1;
    char** argv =  new char*[1]{(char*) "./runner"};

    LoggerEngine* loggerEngine = new SimpleLoggerEngine();

    Mock(RunnerLogger) logger;
    Mock(TestSuiteGenerator) generator;

    Mock(OperatingSystem) os;
    Mock(RunnerLoggerFactory) loggerFactory;
    Mock(TestSuiteGeneratorFactory) generatorFactory;

    void SetUp() {
        ON_CALL(loggerFactory, create(_)).WillByDefault(Return(&logger));
        ON_CALL(generatorFactory, create(_, _, _, _)).WillByDefault(Return(&generator));
    }
};

TEST_F(RunnerTests, Run_ArgsParsing_Failed) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    EXPECT_THAT(runner.run(2, new char*[2]{(char*) "./runner", (char*) "--blah"}), Ne(0));
}

TEST_F(RunnerTests, Run_Specification_Failed) {
    Runner<ProblemSpec> runner(new BadTestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    EXPECT_CALL(generator, generate(_, _)).Times(0);
    EXPECT_CALL(logger, logSpecificationFailure(SpecificationFailure({"An error"})));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_Successful) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(GenerationResult({})));

    EXPECT_THAT(runner.run(argc, argv), Eq(0));
}

TEST_F(RunnerTests, Run_Generation_Failed) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    ON_CALL(generator, generate(_, _))
            .WillByDefault(Return(GenerationResult({
                    TestGroupGenerationResult(new SimpleFailure("Error"), {})})));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_UseDefaultOptions) {
    Runner<ProblemSpec> runner(new TestSpec(), loggerEngine, &os, &loggerFactory, &generatorFactory);
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
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(0)
            .setSlug("foo")
            .setSolutionCommand("./solution")
            .setTestCasesDir("tc")
            .build()));

    runner.run(argc, argv);
}

TEST_F(RunnerTests, Run_Generation_UseArgsOptions) {
    Runner<ProblemSpecWithConfig> runner(
            new TestSpecWithConfig(), loggerEngine, &os, &loggerFactory, &generatorFactory);
    EXPECT_CALL(generator, generate(_, GeneratorConfigBuilder()
            .setSeed(42)
            .setSlug("bar")
            .setSolutionCommand("\"java Solution\"")
            .setTestCasesDir("testdata")
            .build()));

    runner.run(5, new char*[5]{
            (char*) "./runner",
            (char*) "--seed=42",
            (char*) "--slug=bar",
            (char*) "--solution=\"java Solution\"",
            (char*) "--tc-dir=testdata"});
}

TEST_F(RunnerTests, Run_Deprecated_Compiles) {
    Runner<DeprecatedProblem> runner(argc, argv);
    runner.setGenerator(new DeprecatedGenerator());
    // So that it won't run
    if (false) {
        runner.run();
    }
}

}

#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../generator/MockTestSuiteGenerator.hpp"
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

    class TestSpec : public BaseTestSpec<ProblemSpec> {};

    class BadTestSpec : public BaseTestSpec<ProblemSpec> {
    public:
        CoreSpec buildCoreSpec() {
            throw runtime_error("An error");
        }
    };

    class DeprecatedGenerator : public BaseGenerator<ProblemSpec> {};

    int argc = 1;
    char** argv =  new char*[1]{(char*) "./runner"};

    BaseTestSpec<ProblemSpec>* testSpec = new TestSpec();
    BaseTestSpec<ProblemSpec>* badTestSpec = new BadTestSpec();
    LoggerEngine* loggerEngine = new SimpleLoggerEngine();
    OperatingSystem* os = new UnixOperatingSystem();

    Mock(RunnerLogger) logger;
    Mock(TestSuiteGenerator) generator;

    Mock(RunnerLoggerFactory) loggerFactory;
    Mock(TestSuiteGeneratorFactory) generatorFactory;

    void SetUp() {
        ON_CALL(loggerFactory, create(_)).WillByDefault(Return(&logger));
        ON_CALL(generatorFactory, create(_, _, _, _)).WillByDefault(Return(&generator));
    }
};

TEST_F(RunnerTests, Run_ArgsParsing_Failed) {
    Runner<ProblemSpec> runner(testSpec, loggerEngine, os, &loggerFactory, &generatorFactory);
    EXPECT_THAT(runner.run(2, new char*[2]{(char*) "./runner", (char*) "--blah"}), Ne(0));
}

TEST_F(RunnerTests, Run_Specification_Failed) {
    Runner<ProblemSpec> runner(badTestSpec, loggerEngine, os, &loggerFactory, &generatorFactory);
    EXPECT_CALL(generator, generate(_, _)).Times(0);
    EXPECT_CALL(logger, logSpecificationFailure(SpecificationFailure({"An error"})));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Generation_Successful) {
    Runner<ProblemSpec> runner(testSpec, loggerEngine, os, &loggerFactory, &generatorFactory);
    ON_CALL(generator, generate(_, _)).WillByDefault(Return(GenerationResult({})));

    EXPECT_THAT(runner.run(argc, argv), Eq(0));
}

TEST_F(RunnerTests, Run_Generation_Failed) {
    Runner<ProblemSpec> runner(testSpec, loggerEngine, os, &loggerFactory, &generatorFactory);
    ON_CALL(generator, generate(_, _))
            .WillByDefault(Return(GenerationResult({
                    TestGroupGenerationResult(new SimpleFailure("Error"), {})})));

    EXPECT_THAT(runner.run(argc, argv), Ne(0));
}

TEST_F(RunnerTests, Run_Deprecated_Compiles) {
    Runner<ProblemSpec> runner(argc, argv);
    runner.setGenerator(new DeprecatedGenerator());
    // So that it won't run
    if (false) {
        runner.run();
    }
}

}

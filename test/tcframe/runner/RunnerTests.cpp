#include "gmock/gmock.h"
#include "../mock.hpp"

#include "../generator/FakeTestCaseGenerationFailure.hpp"
#include "../generator/MockTestSuiteGenerator.hpp"
#include "../generator/MockTestSuiteGeneratorFactory.hpp"
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
    class FakeProblem : public BaseProblem {
    protected:
        void InputFormat() {}
    };

    class FakeGenerator : public BaseGenerator<FakeProblem> {};

    int argc;
    char** argv;

    BaseGenerator<FakeProblem>* generator = new FakeGenerator();
    LoggerEngine* loggerEngine = new SimpleLoggerEngine();
    OperatingSystem* os = new UnixOperatingSystem();

    Mock(RunnerLogger) logger;
    Mock(TestSuiteGenerator) testSuiteGenerator;

    Mock(RunnerLoggerFactory) loggerFactory;
    Mock(TestSuiteGeneratorFactory) testSuiteGeneratorFactory;

    void SetUp() {
        ON_CALL(loggerFactory, create(_)).WillByDefault(Return(&logger));
        ON_CALL(testSuiteGeneratorFactory, create(_, _, _, _)).WillByDefault(Return(&testSuiteGenerator));
    }
};

TEST_F(RunnerTests, Run_Generation_Successful) {
    argc = 1;
    argv = new char*[1]{(char*) "./runner"};

    Runner<FakeProblem> runner(argc, argv, generator, loggerEngine, os, &loggerFactory, &testSuiteGeneratorFactory);
    ON_CALL(testSuiteGenerator, generate(_, _, _)).WillByDefault(Return(GenerationResult({})));

    EXPECT_THAT(runner.run(), Eq(0));
}

TEST_F(RunnerTests, Run_Generation_Failed) {
    argc = 1;
    argv = new char*[1]{(char*) "./runner"};

    Runner<FakeProblem> runner(argc, argv, generator, loggerEngine, os, &loggerFactory, &testSuiteGeneratorFactory);
    ON_CALL(testSuiteGenerator, generate(_, _, _))
            .WillByDefault(Return(GenerationResult({
                    {"foo_1", TestCaseGenerationResult::failedResult(new FakeTestCaseGenerationFailure())}})));

    EXPECT_THAT(runner.run(), Ne(0));
}

}

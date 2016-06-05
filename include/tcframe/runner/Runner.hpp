#pragma once

#include "RunnerLogger.hpp"
#include "RunnerLoggerFactory.hpp"
#include "SpecificationFailure.hpp"
#include "tcframe/core.hpp"
#include "tcframe/io.hpp"
#include "tcframe/generator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/variable.hpp"
#include "tcframe/verifier.hpp"

namespace tcframe {

template<typename TProblem>
class Runner {
private:
    int argc_;
    char** argv_;

    BaseProblem* problem_;
    BaseGenerator<TProblem>* generator_;

    LoggerEngine* loggerEngine_;
    OperatingSystem* os_;

    RunnerLoggerFactory* runnerLoggerFactory_;
    TestSuiteGeneratorFactory* testSuiteGeneratorFactory_;

public:
    Runner(int argc, char** argv)
            : argc_(argc)
            , argv_(argv)
            , problem_(nullptr)
            , generator_(nullptr)
            , loggerEngine_(new SimpleLoggerEngine())
            , os_(new UnixOperatingSystem())
            , runnerLoggerFactory_(new RunnerLoggerFactory())
            , testSuiteGeneratorFactory_(new TestSuiteGeneratorFactory())
    {}

    /* Visible for testing */
    Runner(
            int argc,
            char** argv,
            BaseGenerator<TProblem>* generator,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            RunnerLoggerFactory* runnerLoggerFactory,
            TestSuiteGeneratorFactory* testSuiteGeneratorFactory)
            : argc_(argc)
            , argv_(argv)
            , problem_(generator)
            , generator_(generator)
            , loggerEngine_(loggerEngine)
            , os_(os)
            , runnerLoggerFactory_(runnerLoggerFactory)
            , testSuiteGeneratorFactory_(testSuiteGeneratorFactory)
    {}

    void setGenerator(BaseGenerator<TProblem>* generator) {
        problem_ = generator;
        generator_ = generator;
    }

    int run() {
        RunnerLogger* logger = runnerLoggerFactory_->create(loggerEngine_);

        ProblemConfig problemConfig;
        GeneratorConfig generatorConfig;
        ConstraintSuite constraintSuite;
        TestSuite testSuite;
        IOFormat ioFormat;

        try {
            problemConfig = problem_->buildProblemConfig();
            generatorConfig = generator_->buildGeneratorConfig();
            constraintSuite = problem_->buildConstraintSuite();
            testSuite = generator_->buildTestSuite();
            ioFormat = problem_->buildIOFormat();
        } catch (runtime_error& e) {
            logger->logSpecificationFailure(SpecificationFailure({e.what()}));
            return 1;
        }

        auto ioManipulator = new IOManipulator(ioFormat);
        auto verifier = new ConstraintSuiteVerifier(constraintSuite);
        auto generationLogger = new GeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os_);

        auto generator = testSuiteGeneratorFactory_->create(testCaseGenerator, ioManipulator, os_, generationLogger);

        return generator->generate(testSuite, problemConfig, generatorConfig).isSuccessful() ? 0 : 1;
    }
};

}

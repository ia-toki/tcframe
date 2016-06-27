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

template<typename TProblemSpec>
class Runner {

    /* For backward compatibility with 0.x versions. */
    typedef TProblemSpec TProblem;

private:
    int argc_;        // Member fields
    char** argv_;     // for backward compatibility with 0.x versions.

    BaseTestSpec<TProblemSpec>* testSpec_;

    LoggerEngine* loggerEngine_;
    OperatingSystem* os_;

    RunnerLoggerFactory* loggerFactory_;
    TestSuiteGeneratorFactory* generatorFactory_;

public:
    Runner(BaseTestSpec<TProblemSpec>* testSpec)
            : testSpec_(testSpec)
            , loggerEngine_(new SimpleLoggerEngine())
            , os_(new UnixOperatingSystem())
            , loggerFactory_(new RunnerLoggerFactory())
            , generatorFactory_(new TestSuiteGeneratorFactory())
    {}

    /* Visible for testing. */
    Runner(
            BaseTestSpec<TProblemSpec>* testSpec,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            RunnerLoggerFactory* runnerLoggerFactory,
            TestSuiteGeneratorFactory* testSuiteGeneratorFactory)
            : testSpec_(testSpec)
            , loggerEngine_(loggerEngine)
            , os_(os)
            , loggerFactory_(runnerLoggerFactory)
            , generatorFactory_(testSuiteGeneratorFactory)
    {}

    /* DEPRECATED. For backward compatibility with 0.x versions. */
    Runner(int argc, char** argv)
            : argc_(argc)
            , argv_(argv)
            , testSpec_(nullptr)
            , loggerEngine_(new SimpleLoggerEngine())
            , os_(new UnixOperatingSystem())
            , loggerFactory_(new RunnerLoggerFactory())
            , generatorFactory_(new TestSuiteGeneratorFactory())
    {}

    /* DEPRECATED. For backward compatibility with 0.x versions. */
    void setGenerator(BaseGenerator<TProblem>* generator) {
        testSpec_ = generator;
    }

    /* DEPRECATED. For backward compatibility with 0.x versions. */
    int run() {
        return run(argc_, argv_);
    }

    int run(int argc, char** argv) {
        auto logger = loggerFactory_->create(loggerEngine_);

        CoreSpec coreSpec;
        try {
            coreSpec = testSpec_->buildCoreSpec();
        } catch (runtime_error& e) {
            logger->logSpecificationFailure(SpecificationFailure({e.what()}));
            return 1;
        }

        return generate(coreSpec);
    }

private:
    int generate(const CoreSpec& coreSpec) {
        auto ioManipulator = new IOManipulator(coreSpec.ioFormat());
        auto verifier = new ConstraintSuiteVerifier(coreSpec.constraintSuite());
        auto logger = new GeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os_, logger);
        auto testGroupGenerator = new TestGroupGenerator(testCaseGenerator, verifier, os_, logger);

        auto generator = generatorFactory_->create(testGroupGenerator, ioManipulator, os_, logger);
        return generator->generate(coreSpec.testSuite(), coreSpec.coreConfig()).isSuccessful() ? 0 : 1;
    }
};

}

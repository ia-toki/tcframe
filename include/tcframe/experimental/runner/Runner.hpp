#pragma once

#include "tcframe/experimental/generation/TestSuiteConfig.hpp"
#include "tcframe/experimental/generation/TestSuiteGenerator.hpp"
#include "tcframe/experimental/os/UnixOperatingSystem.hpp"
#include "tcframe/experimental/runner/BaseGenerator.hpp"
#include "tcframe/experimental/runner/GeneratorConfig.hpp"
#include "tcframe/experimental/runner/ProblemConfig.hpp"
#include "tcframe/experimental/variable/IOVariablePrinter.hpp"

namespace tcframe { namespace experimental {

template<typename TProblem>
class Runner {
private:
    int argc;
    char** argv;
    BaseGenerator<TProblem>* generator_;

public:
    Runner(int argc, char **argv)
            : argc(argc)
            , argv(argv)
            , generator_(nullptr)
    {}

    void setGenerator(BaseGenerator<TProblem>* generator) {
        generator_ = generator;
    }

    int run() {
        ProblemConfig problemConfig = generator_->buildProblemConfig();
        GeneratorConfig generatorConfig = generator_->buildGeneratorConfig();
        TestSuiteConfig testSuiteConfig = TestSuiteConfigBuilder()
                .setSlug(problemConfig.slug())
                .setTestCasesDir(generatorConfig.testCasesDir())
                .setSolutionCommand("./solution")
                .build();
        TestSuite testSuite = generator_->buildTestSuite();
        IOFormat ioFormat = generator_->buildIOFormat();

        TestSuiteGenerator* testSuiteGenerator = new TestSuiteGenerator(
                new IOVariablePrinter(ioFormat),
                new UnixOperatingSystem(),
                testSuiteConfig);

        testSuiteGenerator->generate(testSuite);

        return 0;
    }
};

}}

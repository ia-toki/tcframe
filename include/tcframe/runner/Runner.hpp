#pragma once

#include "tcframe/config/GeneratorConfig.hpp"
#include "tcframe/config/ProblemConfig.hpp"
#include "tcframe/generation/TestSuiteGenerationListener.hpp"
#include "tcframe/generation/TestSuiteGenerator.hpp"
#include "tcframe/generation/TestCaseGenerator.hpp"
#include "tcframe/os/OperatingSystem.hpp"
#include "tcframe/os/UnixOperatingSystem.hpp"
#include "tcframe/runner/BaseGenerator.hpp"
#include "tcframe/runner/BaseProblem.hpp"
#include "tcframe/variable/IOVariablePrinter.hpp"
#include "tcframe/verification/ConstraintSuiteVerifier.hpp"

namespace tcframe { namespace experimental {

template<typename TProblem>
class Runner {
private:
    int argc;
    char** argv;
    BaseProblem* problem_;
    BaseGenerator<TProblem>* generator_;

public:
    Runner(int argc, char **argv)
            : argc(argc)
            , argv(argv)
            , generator_(nullptr)
    {}

    void setGenerator(BaseGenerator<TProblem>* generator) {
        problem_ = generator;
        generator_ = generator;
    }

    int run() {
        auto problemConfig = problem_->buildProblemConfig();
        auto ioFormat = problem_->buildIOFormat();
        auto constraintSuite = problem_->buildConstraintSuite();

        auto generatorConfig = generator_->buildGeneratorConfig();
        auto testSuite = generator_->buildTestSuite();

        auto os = new UnixOperatingSystem();
        auto ioVariablePrinter = new IOVariablePrinter(ioFormat);
        auto constraintSuiteVerifier = new ConstraintSuiteVerifier(constraintSuite);
        auto generationListener = new TestSuiteGenerationListener();
        auto testCaseGenerator = new TestCaseGenerator(constraintSuiteVerifier, ioVariablePrinter, os);
        auto testSuiteGenerator = new TestSuiteGenerator(testCaseGenerator, os, generationListener);

        return testSuiteGenerator->generate(testSuite, problemConfig, generatorConfig).isSuccessful() ? 0 : 1;
    }
};

}}

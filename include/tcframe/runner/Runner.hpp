#pragma once

#include "BaseGenerator.hpp"
#include "BaseProblem.hpp"
#include "tcframe/config.hpp"
#include "tcframe/io.hpp"
#include "tcframe/generation.hpp"
#include "tcframe/os.hpp"
#include "tcframe/variable.hpp"
#include "tcframe/verification.hpp"

namespace tcframe {

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
        auto ioManipulator = new IOManipulator(ioFormat);
        auto constraintSuiteVerifier = new ConstraintSuiteVerifier(constraintSuite);
        auto generationListener = new TestSuiteGenerationListener();
        auto testCaseGenerator = new TestCaseGenerator(constraintSuiteVerifier, ioManipulator, os);
        auto testSuiteGenerator = new TestSuiteGenerator(testCaseGenerator, ioManipulator, os, generationListener);

        return testSuiteGenerator->generate(testSuite, problemConfig, generatorConfig).isSuccessful() ? 0 : 1;
    }
};

}

#pragma once

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
        auto verifier = new Verifier(constraintSuite);
        auto generationLogger = new GenerationLogger();
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os);
        auto generator = new Generator(testCaseGenerator, ioManipulator, os, generationLogger);

        return generator->generate(testSuite, problemConfig, generatorConfig).isSuccessful() ? 0 : 1;
    }
};

}

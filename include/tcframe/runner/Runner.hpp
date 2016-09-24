#pragma once

#include <iostream>

#include "Args.hpp"
#include "ArgsParser.hpp"
#include "RunnerLogger.hpp"
#include "RunnerLoggerFactory.hpp"
#include "tcframe/generator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/submitter.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verifier.hpp"

using std::cout;
using std::endl;

namespace tcframe {

template<typename TProblemSpec>
class Runner {
private:
    BaseTestSpec<TProblemSpec>* testSpec_;

    LoggerEngine* loggerEngine_;
    OperatingSystem* os_;

    RunnerLoggerFactory* loggerFactory_;
    GeneratorFactory* generatorFactory_;
    SubmitterFactory* submitterFactory_;

public:
    Runner(BaseTestSpec<TProblemSpec>* testSpec)
            : testSpec_(testSpec)
            , loggerEngine_(new SimpleLoggerEngine())
            , os_(new UnixOperatingSystem())
            , loggerFactory_(new RunnerLoggerFactory())
            , generatorFactory_(new GeneratorFactory())
            , submitterFactory_(new SubmitterFactory()) {}

    /* Visible for testing. */
    Runner(
            BaseTestSpec<TProblemSpec>* testSpec,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            RunnerLoggerFactory* runnerLoggerFactory,
            GeneratorFactory* generatorFactory,
            SubmitterFactory* submitterFactory)
            : testSpec_(testSpec)
            , loggerEngine_(loggerEngine)
            , os_(os)
            , loggerFactory_(runnerLoggerFactory)
            , generatorFactory_(generatorFactory)
            , submitterFactory_(submitterFactory) {}

    int run(int argc, char* argv[]) {
        auto logger = loggerFactory_->create(loggerEngine_);

        try {
            Args args = parseArgs(argc, argv);
            CoreSpec coreSpec = buildCoreSpec(logger);

            if (args.command() == Args::Command::GEN) {
                return generate(args, coreSpec);
            } else {
                return submit(args, coreSpec);
            }
        } catch (...) {
            return 1;
        }
    }

private:
    Args parseArgs(int argc, char* argv[]) {
        try {
            return ArgsParser::parse(argc, argv);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            throw;
        }
    }

    CoreSpec buildCoreSpec(RunnerLogger* logger) {
        try {
            return testSpec_->buildCoreSpec();
        } catch (runtime_error& e) {
            logger->logSpecificationFailure({e.what()});
            throw;
        }
    }

    int generate(const Args& args, const CoreSpec& coreSpec) {
        const ProblemConfig& problemConfig = coreSpec.problemConfig();

        GeneratorConfig config = GeneratorConfigBuilder()
                .setMultipleTestCasesCount(problemConfig.multipleTestCasesCount().value_or(nullptr))
                .setSeed(args.seed().value_or(DefaultValues::seed()))
                .setSlug(problemConfig.slug().value_or(DefaultValues::slug()))
                .setSolutionCommand(args.solution().value_or(DefaultValues::solutionCommand()))
                .setTestCasesDir(args.tcDir().value_or(DefaultValues::testCasesDir()))
                .build();

        auto ioManipulator = new IOManipulator(coreSpec.ioFormat());
        auto verifier = new Verifier(coreSpec.constraintSuite());
        auto logger = new GeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os_, logger);
        auto generator = generatorFactory_->create(testCaseGenerator, verifier, os_, logger);

        return generator->generate(coreSpec.testSuite(), config) ? 0 : 1;
    }

    int submit(const Args& args, const CoreSpec& coreSpec) {
        const ProblemConfig& problemConfig = coreSpec.problemConfig();

        SubmitterConfigBuilder configBuilder = SubmitterConfigBuilder()
                .setHasMultipleTestCasesCount(problemConfig.multipleTestCasesCount())
                .setSlug(problemConfig.slug().value_or(DefaultValues::slug()))
                .setSolutionCommand(args.solution().value_or(DefaultValues::solutionCommand()))
                .setTestCasesDir(args.tcDir().value_or(DefaultValues::testCasesDir()));

        if (!args.noTimeLimit()) {
            if (args.timeLimit()) {
                configBuilder.setTimeLimit(args.timeLimit().value());
            } else if (problemConfig.timeLimit()) {
                configBuilder.setTimeLimit(problemConfig.timeLimit().value());
            }
        }
        if (!args.noMemoryLimit()) {
            if (args.memoryLimit()) {
                configBuilder.setMemoryLimit(args.memoryLimit().value());
            } else if (problemConfig.memoryLimit()) {
                configBuilder.setMemoryLimit(problemConfig.memoryLimit().value());
            }
        }

        SubmitterConfig config = configBuilder.build();

        auto logger = new SubmitterLogger(loggerEngine_);
        auto evaluator = new BatchEvaluator(os_, logger);
        auto scorer = new DiffScorer(os_, logger);
        auto testCaseSubmitter = new TestCaseSubmitter(evaluator, scorer, logger);
        auto submitter = submitterFactory_->create(testCaseSubmitter, logger);

        set<int> subtaskIds;
        for (const Subtask& subtask : coreSpec.constraintSuite().constraints()) {
            subtaskIds.insert(subtask.id());
        }

        submitter->submit(coreSpec.testSuite(), subtaskIds, config);
        return 0;
    }
};

}

#pragma once

#include <iostream>

#include "Args.hpp"
#include "ArgsParser.hpp"
#include "ConfigParser.hpp"
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

    ConfigParser* configParser_;
    RunnerLoggerFactory* runnerLoggerFactory_;
    GeneratorFactory* generatorFactory_;
    SubmitterFactory* submitterFactory_;

public:
    Runner(
            BaseTestSpec<TProblemSpec>* testSpec,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            ConfigParser* configParser,
            RunnerLoggerFactory* runnerLoggerFactory,
            GeneratorFactory* generatorFactory,
            SubmitterFactory* submitterFactory)
            : testSpec_(testSpec)
            , loggerEngine_(loggerEngine)
            , configParser_(configParser)
            , os_(os)
            , runnerLoggerFactory_(runnerLoggerFactory)
            , generatorFactory_(generatorFactory)
            , submitterFactory_(submitterFactory) {}

    int run(int argc, char* argv[]) {
        auto runnerLogger = runnerLoggerFactory_->create(loggerEngine_);

        try {
            Args args = parseArgs(argc, argv);
            Config config = parseConfig(argv[0]);
            Spec spec = buildSpec(config, runnerLogger);

            int result;
            if (args.command() == Args::Command::GEN) {
                result = generate(args, spec);
            } else {
                result = submit(args, spec);
            }
            cleanUp();
            return result;
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

    Config parseConfig(const string& runnerPath) {
        try {
            return configParser_->parse(runnerPath);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            throw;
        }
    }

    Spec buildSpec(const Config& config, RunnerLogger* logger) {
        try {
            return testSpec_->buildSpec(config);
        } catch (runtime_error& e) {
            logger->logSpecificationFailure({e.what()});
            throw;
        }
    }

    int generate(const Args& args, const Spec& spec) {
        const Config& config = spec.config();
        const ProblemConfig& problemConfig = spec.problemConfig();

        GeneratorConfig generatorConfig = GeneratorConfigBuilder()
                .setMultipleTestCasesCount(problemConfig.multipleTestCasesCount().value_or(nullptr))
                .setSeed(args.seed().value_or(DefaultValues::seed()))
                .setSlug(config.slug())
                .setSolutionCommand(args.solution().value_or(DefaultValues::solutionCommand()))
                .setOutputDir(args.output().value_or(DefaultValues::outputDir()))
                .build();

        auto ioManipulator = new IOManipulator(spec.ioFormat());
        auto verifier = new Verifier(spec.constraintSuite());
        auto logger = new GeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os_, logger);
        auto generator = generatorFactory_->create(testCaseGenerator, verifier, os_, logger);

        return generator->generate(spec.testSuite(), generatorConfig) ? 0 : 1;
    }

    int submit(const Args& args, const Spec& spec) {
        const Config& config = spec.config();
        const ProblemConfig& problemConfig = spec.problemConfig();

        SubmitterConfigBuilder configBuilder = SubmitterConfigBuilder()
                .setHasMultipleTestCasesCount(problemConfig.multipleTestCasesCount())
                .setSlug(config.slug())
                .setSolutionCommand(args.solution().value_or(DefaultValues::solutionCommand()))
                .setTestCasesDir(args.output().value_or(DefaultValues::outputDir()));

        if (!args.noTimeLimit()) {
            if (args.timeLimit()) {
                configBuilder.setTimeLimit(args.timeLimit().value());
            } else if (config.timeLimit()) {
                configBuilder.setTimeLimit(config.timeLimit().value());
            }
        }
        if (!args.noMemoryLimit()) {
            if (args.memoryLimit()) {
                configBuilder.setMemoryLimit(args.memoryLimit().value());
            } else if (config.memoryLimit()) {
                configBuilder.setMemoryLimit(config.memoryLimit().value());
            }
        }

        SubmitterConfig submitterConfig = configBuilder.build();

        auto logger = new SubmitterLogger(loggerEngine_);
        auto evaluator = new BatchEvaluator(os_, logger);
        auto scorer = new DiffScorer(os_, logger);
        auto testCaseSubmitter = new TestCaseSubmitter(evaluator, scorer, logger);
        auto submitter = submitterFactory_->create(testCaseSubmitter, logger);

        set<int> subtaskIds;
        for (const Subtask& subtask : spec.constraintSuite().constraints()) {
            subtaskIds.insert(subtask.id());
        }

        submitter->submit(spec.testSuite(), subtaskIds, submitterConfig);
        return 0;
    }

    void cleanUp() {
        os_->execute(ExecutionRequestBuilder().setCommand("rm _*.out").build());
    }
};

}

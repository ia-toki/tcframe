#pragma once

#include <iostream>

#include "Args.hpp"
#include "ArgsParser.hpp"
#include "tcframe/os.hpp"
#include "tcframe/runner/aggregator.hpp"
#include "tcframe/runner/client.hpp"
#include "tcframe/runner/evaluator.hpp"
#include "tcframe/runner/generator.hpp"
#include "tcframe/runner/grader.hpp"
#include "tcframe/runner/logger.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"

using std::cout;
using std::endl;

namespace tcframe {

struct RunnerDefaults {
    static constexpr unsigned SEED = 0;
    static constexpr const char* OUTPUT_DIR = "tc";
    static constexpr const char* SOLUTION_COMMAND = "./solution";
    static constexpr const char* SCORER_COMMAND = "./scorer";
    static constexpr const char* COMMUNICATOR_COMMAND = "./communicator";
};

template<typename TProblemSpec>
class Runner {
private:
    Driver<TProblemSpec>* driver_;

    LoggerEngine* loggerEngine_;
    OperatingSystem* os_;

    RunnerLoggerFactory* runnerLoggerFactory_;
    GraderLoggerFactory* graderLoggerFactory_;
    GeneratorFactory* generatorFactory_;
    GraderFactory* graderFactory_;
    EvaluatorRegistry* evaluatorRegistry_;
    AggregatorRegistry* aggregatorRegistry_;

public:
    Runner(
            Driver<TProblemSpec>* driver,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            RunnerLoggerFactory* runnerLoggerFactory,
            GraderLoggerFactory* graderLoggerFactory,
            GeneratorFactory* generatorFactory,
            GraderFactory* graderFactory,
            EvaluatorRegistry* evaluatorRegistry,
            AggregatorRegistry* aggregatorRegistry)
            : driver_(driver)
            , loggerEngine_(loggerEngine)
            , os_(os)
            , runnerLoggerFactory_(runnerLoggerFactory)
            , graderLoggerFactory_(graderLoggerFactory)
            , generatorFactory_(generatorFactory)
            , graderFactory_(graderFactory)
            , evaluatorRegistry_(evaluatorRegistry)
            , aggregatorRegistry_(aggregatorRegistry) {}

    int run(int argc, char* argv[]) {
        auto runnerLogger = runnerLoggerFactory_->create(loggerEngine_);

        try {
            Args args = parseArgs(argc, argv);
            Spec spec = buildSpec(runnerLogger);

            int result;
            if (args.command() == Args::Command::GENERATE) {
                result = generate(args, spec);
            } else {
                result = grade(args, spec);
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

    Spec buildSpec(RunnerLogger* runnerLogger) {
        try {
            return driver_->buildSpec();
        } catch (runtime_error& e) {
            runnerLogger->logSpecificationFailure({e.what()});
            throw;
        }
    }

    int generate(const Args& args, const Spec& spec) {
        const MultipleTestCasesConfig& multipleTestCasesConfig = spec.multipleTestCasesConfig();

        GenerationOptionsBuilder optionsBuilder = GenerationOptionsBuilder(spec.slug())
                .setMultipleTestCasesCounter(multipleTestCasesConfig.counter())
                .setMultipleTestCasesOutputPrefix(multipleTestCasesConfig.outputPrefix())
                .setSeed(args.seed().value_or(unsigned(RunnerDefaults::SEED)))
                .setSolutionCommand(args.solution().value_or(string(RunnerDefaults::SOLUTION_COMMAND)))
                .setOutputDir(args.output().value_or(string(RunnerDefaults::OUTPUT_DIR)));

        EvaluatorConfig evaluatorConfig = evaluatorRegistry_->getConfig(spec.styleConfig().evaluationStyle());
        if (evaluatorConfig.testCaseOutputType() == TestCaseOutputType::NOT_REQUIRED) {
            optionsBuilder.setNeedsOutput(false);
        } else {
            optionsBuilder.setNeedsOutput(spec.styleConfig().needsOutput());
        }

        GenerationOptions options = optionsBuilder.build();

        auto helperCommands = getHelperCommands(args, spec.styleConfig());
        auto evaluator = evaluatorRegistry_->get(spec.styleConfig().evaluationStyle(), os_, helperCommands);
        auto logger = new DefaultGeneratorLogger(loggerEngine_);
        auto specClient = new SpecClient(driver_->getSpecDriver(spec), os_);
        auto testCaseGenerator = new TestCaseGenerator(specClient, evaluator, logger);
        auto generator = generatorFactory_->create(spec.seedSetter(), specClient, testCaseGenerator, os_, logger);

        return generator->generate(options) ? 0 : 1;
    }

    int grade(const Args& args, const Spec& spec) {
        const MultipleTestCasesConfig& multipleTestCasesConfig = spec.multipleTestCasesConfig();
        const GradingConfig& gradingConfig = spec.gradingConfig();

        GradingOptionsBuilder optionsBuilder = GradingOptionsBuilder(spec.slug())
                .setHasMultipleTestCases((bool) multipleTestCasesConfig.counter())
                .setSolutionCommand(args.solution().value_or(string(RunnerDefaults::SOLUTION_COMMAND)))
                .setOutputDir(args.output().value_or(string(RunnerDefaults::OUTPUT_DIR)));

        if (!args.noTimeLimit()) {
            optionsBuilder.setTimeLimit(args.timeLimit().value_or(gradingConfig.timeLimit()));
        }
        if (!args.noMemoryLimit()) {
            optionsBuilder.setMemoryLimit(args.memoryLimit().value_or(gradingConfig.memoryLimit()));
        }

        GradingOptions options = optionsBuilder.build();

        auto logger = graderLoggerFactory_->create(loggerEngine_, args.brief());
        auto helperCommands = getHelperCommands(args, spec.styleConfig());
        auto evaluator = evaluatorRegistry_->get(spec.styleConfig().evaluationStyle(), os_, helperCommands);
        auto specClient = new SpecClient(driver_->getSpecDriver(spec), os_);
        auto testCaseGrader = new TestCaseGrader(evaluator, logger);
        auto aggregator = aggregatorRegistry_->get(spec.constraintSuite().hasSubtasks());
        auto grader = graderFactory_->create(specClient, testCaseGrader, aggregator, logger);

        grader->grade(spec.constraintSuite(), options);
        return 0;
    }

    void cleanUp() {
        os_->execute(ExecutionRequestBuilder().setCommand("rm __tcframe_*").build());
    }

    static map<string, string> getHelperCommands(const Args& args, const StyleConfig& styleConfig) {
        map<string, string> helperCommands;
        if (styleConfig.needsCustomScorer()) {
            helperCommands["scorer"] = args.scorer().value_or(string(RunnerDefaults::SCORER_COMMAND));
        }
        helperCommands["communicator"] = args.communicator().value_or(string(RunnerDefaults::COMMUNICATOR_COMMAND));
        return helperCommands;
    };
};

}

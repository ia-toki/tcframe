#pragma once

#include <iostream>
#include <utility>

#include "Args.hpp"
#include "ArgsParser.hpp"
#include "tcframe/runner/aggregator.hpp"
#include "tcframe/runner/client.hpp"
#include "tcframe/runner/evaluator.hpp"
#include "tcframe/runner/generator.hpp"
#include "tcframe/runner/grader.hpp"
#include "tcframe/runner/logger.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"

using std::cout;
using std::endl;
using std::pair;

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
            pair<SpecYaml, SpecDriver*> spec = buildSpec(runnerLogger);
            auto specClient = new SpecClient(spec.second, os_);

            int result;
            if (args.command() == Args::Command::GENERATE) {
                result = generate(args, spec.first, specClient);
            } else {
                result = grade(args, spec.first, specClient);
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

    pair<SpecYaml, SpecDriver*> buildSpec(RunnerLogger* runnerLogger) {
        try {
            return driver_->buildSpec();
        } catch (runtime_error& e) {
            runnerLogger->logSpecificationFailure({e.what()});
            throw;
        }
    }

    int generate(const Args& args, const SpecYaml& spec, SpecClient* specClient) {
        auto optionsBuilder = GenerationOptionsBuilder(spec.slug)
                .setSeed(args.seed().value_or(unsigned(RunnerDefaults::SEED)))
                .setSolutionCommand(args.solution().value_or(string(RunnerDefaults::SOLUTION_COMMAND)))
                .setOutputDir(args.output().value_or(string(RunnerDefaults::OUTPUT_DIR)));

        EvaluatorConfig evaluatorConfig = evaluatorRegistry_->getConfig(spec.evaluator.slug);
        if (evaluatorConfig.testCaseOutputType() == TestCaseOutputType::NOT_REQUIRED) {
            optionsBuilder.setHasTcOutput(false);
        } else {
            optionsBuilder.setHasTcOutput(spec.evaluator.has_tc_output);
        }

        GenerationOptions options = optionsBuilder.build();

        auto helperCommands = getHelperCommands(args, spec.evaluator.has_scorer);
        auto evaluator = evaluatorRegistry_->get(spec.evaluator.slug, os_, helperCommands);
        auto logger = new DefaultGeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(specClient, evaluator, logger);
        auto generator = generatorFactory_->create(specClient, testCaseGenerator, os_, logger);

        return generator->generate(options) ? 0 : 1;
    }

    int grade(const Args& args, const SpecYaml& spec, SpecClient* specClient) {
        auto optionsBuilder = GradingOptionsBuilder(spec.slug)
                .setSolutionCommand(args.solution().value_or(string(RunnerDefaults::SOLUTION_COMMAND)))
                .setOutputDir(args.output().value_or(string(RunnerDefaults::OUTPUT_DIR)));

        if (!args.noTimeLimit()) {
            optionsBuilder.setTimeLimit(args.timeLimit().value_or(spec.limits.time_s));
        }
        if (!args.noMemoryLimit()) {
            optionsBuilder.setMemoryLimit(args.memoryLimit().value_or(spec.limits.memory_mb));
        }

        vector<double> subtaskPoints;
        for (const SubtaskYaml& subtask : spec.subtasks) {
            subtaskPoints.push_back(subtask.points);
        }
        optionsBuilder.setSubtaskPoints(subtaskPoints);

        GradingOptions options = optionsBuilder.build();

        auto logger = graderLoggerFactory_->create(loggerEngine_, args.brief());
        auto helperCommands = getHelperCommands(args, spec.evaluator.has_scorer);
        auto evaluator = evaluatorRegistry_->get(spec.evaluator.slug, os_, helperCommands);
        auto testCaseGrader = new TestCaseGrader(evaluator, logger);
        auto aggregator = aggregatorRegistry_->get(!spec.subtasks.empty());
        auto grader = graderFactory_->create(specClient, testCaseGrader, aggregator, logger);

        grader->grade(options);
        return 0;
    }

    void cleanUp() {
        os_->execute(ExecutionRequestBuilder().setCommand("rm __tcframe_*").build());
    }

    static map<string, string> getHelperCommands(const Args& args, bool hasScorer) {
        map<string, string> helperCommands;
        if (hasScorer) {
            helperCommands["scorer"] = args.scorer().value_or(string(RunnerDefaults::SCORER_COMMAND));
        }
        helperCommands["communicator"] = args.communicator().value_or(string(RunnerDefaults::COMMUNICATOR_COMMAND));
        return helperCommands;
    };
};

}

#pragma once

#include <iostream>

#include "Args.hpp"
#include "ArgsParser.hpp"
#include "RunnerLogger.hpp"
#include "SlugParser.hpp"
#include "tcframe/aggregator.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/generator.hpp"
#include "tcframe/grader.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verifier.hpp"

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
    string specPath_;

    BaseTestSpec<TProblemSpec>* testSpec_;

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
            const string& specPath,
            BaseTestSpec<TProblemSpec>* testSpec,
            LoggerEngine* loggerEngine,
            OperatingSystem* os,
            RunnerLoggerFactory* runnerLoggerFactory,
            GraderLoggerFactory* graderLoggerFactory,
            GeneratorFactory* generatorFactory,
            GraderFactory* graderFactory,
            EvaluatorRegistry* evaluatorRegistry,
            AggregatorRegistry* aggregatorRegistry)
            : specPath_(specPath)
            , testSpec_(testSpec)
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
            string slug = parseSlug();
            Args args = parseArgs(argc, argv);
            Spec spec = buildSpec(slug, runnerLogger);

            int result;
            if (args.command() == Args::Command::GENERATE) {
                result = generate(slug, args, spec);
            } else {
                result = grade(slug, args, spec);
            }
            cleanUp();
            return result;
        } catch (...) {
            return 1;
        }
    }

private:
    string parseSlug() {
        try {
            return SlugParser::parse(specPath_);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            throw;
        }
    }

    Args parseArgs(int argc, char* argv[]) {
        try {
            return ArgsParser::parse(argc, argv);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            throw;
        }
    }

    Spec buildSpec(const string& slug, RunnerLogger* runnerLogger) {
        try {
            return testSpec_->buildSpec(slug);
        } catch (runtime_error& e) {
            runnerLogger->logSpecificationFailure({e.what()});
            throw;
        }
    }

    int generate(const string& slug, const Args& args, const Spec& spec) {
        const MultipleTestCasesConfig& multipleTestCasesConfig = spec.multipleTestCasesConfig();

        GenerationOptionsBuilder optionsBuilder = GenerationOptionsBuilder(slug)
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

        auto ioManipulator = new IOManipulator(spec.ioFormat());
        auto verifier = new Verifier(spec.constraintSuite());
        auto helperCommands = getHelperCommands(args, spec.styleConfig());
        auto evaluator = evaluatorRegistry_->get(spec.styleConfig().evaluationStyle(), os_, helperCommands);
        auto logger = new DefaultGeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(verifier, ioManipulator, os_, evaluator, logger);
        auto generator = generatorFactory_->create(spec.seedSetter(), testCaseGenerator, verifier, os_, logger);

        return generator->generate(spec.testSuite(), options) ? 0 : 1;
    }

    int grade(const string& slug, const Args& args, const Spec& spec) {
        const MultipleTestCasesConfig& multipleTestCasesConfig = spec.multipleTestCasesConfig();
        const GradingConfig& gradingConfig = spec.gradingConfig();

        GradingOptionsBuilder optionsBuilder = GradingOptionsBuilder(slug)
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
        auto testCaseGrader = new TestCaseGrader(evaluator, logger);
        auto aggregator = aggregatorRegistry_->get(spec.constraintSuite().hasSubtasks());
        auto grader = graderFactory_->create(testCaseGrader, aggregator, logger);

        grader->grade(spec.testSuite(), spec.constraintSuite(), options);
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

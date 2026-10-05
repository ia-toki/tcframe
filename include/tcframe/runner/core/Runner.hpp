#pragma once

#include <fstream>
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
#include "SolutionArgs.hpp"
#include "SpecOverrides.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::pair;
using std::set;
using std::string;

namespace tcframe {

struct RunnerDefaults {
    static constexpr unsigned SEED = 0;
    static constexpr const char* OUTPUT_DIR = "tc";
    static constexpr const char* SOLUTION_COMMAND = "./solution";
    static constexpr const char* SCORER_COMMAND = "./scorer";
    static constexpr const char* COMMUNICATOR_COMMAND = "./communicator";
    static constexpr const char* MANAGER_DIR = "./manager";
    static constexpr const char* SPEC_FILE = "spec.yml";
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
            if (args.command() == Args::Command::SPEC) {
                return emitSpec(args, spec.first);
            }
            if (args.command() == Args::Command::VALIDATE) {
                return validate(spec.second);
            }

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

    int emitSpec(const Args& args, SpecYaml spec) {
        try {
            SpecOverrides::apply(args, spec);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            return 1;
        }

        string path = args.specFile().value_or(string(RunnerDefaults::SPEC_FILE));
        ofstream out(path);
        if (!out) {
            cout << "tcframe: cannot write spec file '" << path << "'" << endl;
            return 1;
        }

        SpecYamlEmitter::emit(out, spec);
        return out.good() ? 0 : 1;
    }

    // Validator contract (SPEC.md T7.1): reads one test case from stdin. Stdout gets the number of
    // valid subtasks, then one subtask number per line. Invalid input goes to stderr with exit 1.
    // Exit 2 means the spec cannot be validated (multiple test cases, SPEC.md T7.3).
    int validate(SpecDriver* specDriver) {
        if (specDriver->hasMultipleTestCases()) {
            cerr << "tcframe: validator does not support multiple test cases" << endl;
            return 2;
        }

        set<int> subtaskIds;
        try {
            subtaskIds = specDriver->validateTestCaseInput(&cin);
        } catch (FormattedError& e) {
            for (const auto& message : e.messages()) {
                cerr << string(message.first * 2, ' ') << message.second << endl;
            }
            return 1;
        } catch (runtime_error& e) {
            cerr << e.what() << endl;
            return 1;
        }

        cout << subtaskIds.size() << endl;
        for (int subtaskId : subtaskIds) {
            cout << subtaskId << endl;
        }
        return 0;
    }

    int generate(const Args& args, const SpecYaml& spec, SpecClient* specClient) {
        SolutionMap solutions;
        try {
            solutions = buildSolutions(args, spec);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            return 1;
        }

        auto optionsBuilder = GenerationOptionsBuilder(spec.slug)
                .setSeed(args.seed().value_or(unsigned(RunnerDefaults::SEED)))
                .setSolutions(solutions)
                .setOutputDir(args.output().value_or(string(RunnerDefaults::OUTPUT_DIR)));

        EvaluatorConfig evaluatorConfig = evaluatorRegistry_->getConfig(spec.evaluator.slug);
        if (evaluatorConfig.testCaseOutputType() == TestCaseOutputType::NOT_REQUIRED) {
            optionsBuilder.setHasTcOutput(false);
        } else {
            optionsBuilder.setHasTcOutput(spec.evaluator.tc_output_present);
        }

        GenerationOptions options = optionsBuilder.build();

        auto helperCommands = getHelperCommands(args, spec.evaluator.has_scorer);
        auto evaluator = evaluatorRegistry_->get(spec.evaluator.slug, os_, helperCommands, getScorerArgs(spec));
        auto logger = new DefaultGeneratorLogger(loggerEngine_);
        auto testCaseGenerator = new TestCaseGenerator(specClient, evaluator, logger);
        auto generator = generatorFactory_->create(specClient, testCaseGenerator, os_, logger);

        try {
            return generator->generate(options) ? 0 : 1;
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            return 1;
        }
    }

    int grade(const Args& args, const SpecYaml& spec, SpecClient* specClient) {
        SolutionMap solutions;
        try {
            solutions = buildSolutions(args, spec);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            return 1;
        }

        auto optionsBuilder = GradingOptionsBuilder(spec.slug)
                .setSolutions(solutions)
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

        bool json = args.format().value_or("text") == "json";
        auto logger = graderLoggerFactory_->create(loggerEngine_, args.brief(), json);
        auto helperCommands = getHelperCommands(args, spec.evaluator.has_scorer);
        auto evaluator = evaluatorRegistry_->get(spec.evaluator.slug, os_, helperCommands, getScorerArgs(spec));
        auto testCaseGrader = new TestCaseGrader(evaluator, logger);
        auto aggregators = createTestCaseAggregators(spec);
        auto subtaskAggregator = aggregatorRegistry_->getSubtaskAggregator();
        auto grader = graderFactory_->create(specClient, testCaseGrader, aggregators, subtaskAggregator, logger);

        try {
            grader->grade(options);
        } catch (runtime_error& e) {
            cout << e.what() << endl;
            return 1;
        }
        return 0;
    }

    SolutionMap buildSolutions(const Args& args, const SpecYaml& spec) {
        return SolutionArgs::build(args, RunnerDefaults::SOLUTION_COMMAND, spec.evaluator.solution_keys);
    }

    // One test case aggregator per subtask id (1-based, as in Grader), or a single one for the main group.
    map<int, TestCaseAggregator*> createTestCaseAggregators(const SpecYaml& spec) {
        map<int, TestCaseAggregator*> aggregatorsById;
        if (spec.subtasks.empty()) {
            aggregatorsById[Subtask::MAIN_ID] = aggregatorRegistry_->getTestCaseAggregator("", "", false);
            return aggregatorsById;
        }
        for (size_t i = 0; i < spec.subtasks.size(); i++) {
            const AggregatorYaml& aggregator = spec.subtasks[i].aggregator;
            aggregatorsById[i + 1] = aggregatorRegistry_->getTestCaseAggregator(aggregator.slug, aggregator.args, true);
        }
        return aggregatorsById;
    }

    void cleanUp() {
        os_->execute(ExecutionRequestBuilder().setCommand("rm __tcframe_*").build());
    }

    // Scorer options from spec.yml (e.g. float tolerance). Shared by generation and grading,
    // so samples are checked with the same scorer as the official grading.
    static string getScorerArgs(const SpecYaml& spec) {
        return spec.helpers.count("scorer") ? spec.helpers.at("scorer").additional_args : "";
    }

    static map<string, string> getHelperCommands(const Args& args, bool hasScorer) {
        map<string, string> helperCommands;
        if (hasScorer) {
            helperCommands["scorer"] = args.scorer().value_or(string(RunnerDefaults::SCORER_COMMAND));
        }
        helperCommands["communicator"] = args.communicator().value_or(string(RunnerDefaults::COMMUNICATOR_COMMAND));
        helperCommands["manager"] = args.manager().value_or(string(RunnerDefaults::MANAGER_DIR));
        // Not helper programs: the evaluator directory (custom build_/run_ scripts, SPEC.md T6.3)
        // and the solution language family, passed through the same map.
        if (args.evaluatorDir()) {
            helperCommands["evaluator"] = args.evaluatorDir().value();
        }
        helperCommands["solution-family"] = args.solutionFamily().value_or("cpp");
        return helperCommands;
    };
};

}

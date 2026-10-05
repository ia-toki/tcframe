#pragma once

#include <cmath>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include "SlugParser.hpp"
#include "SpecDriver.hpp"
#include "TestCaseDriver.hpp"
#include "tcframe/spec.hpp"

using std::endl;
using std::make_pair;
using std::move;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::string;

namespace tcframe {

template<typename TProblemSpec>
class Driver {
private:
    string specPath_;
    BaseTestSpec<TProblemSpec>* testSpec_;

public:
    virtual ~Driver() = default;

    Driver(
            string specPath,
            BaseTestSpec<TProblemSpec>* testSpec)
            : specPath_(move(specPath))
            , testSpec_(testSpec) {}

    // 2.0 entry point: the runner's `spec` command calls this, then emits spec.yml
    virtual pair<SpecYaml, SpecDriver*> buildSpec() {
        SpecYaml spec;
        spec.slug = SlugParser::parse(specPath_);

        ConstraintSuite constraintSuite = testSpec_->TProblemSpec::buildConstraintSuite();
        if (constraintSuite.hasSubtasks()) {
            for (const Subtask& subtask : constraintSuite.constraints()) {
                if (subtask.id() != Subtask::MAIN_ID) {
                    SubtaskYaml subtaskYaml;
                    subtaskYaml.points = subtask.points();
                    subtaskYaml.aggregator.slug = subtask.aggregator().slug;
                    subtaskYaml.aggregator.args = subtask.aggregator().args;
                    spec.subtasks.push_back(subtaskYaml);
                }
            }
        }

        StyleConfig styleConfig = testSpec_->TProblemSpec::buildStyleConfig();
        switch (styleConfig.evaluationStyle()) {
            case EvaluationStyle::BATCH:
                spec.evaluator.slug = "batch";
                break;
            case EvaluationStyle::INTERACTIVE:
                spec.evaluator.slug = "interactive";
                break;
            case EvaluationStyle::OUTPUT_ONLY:
                spec.evaluator.slug = "output_only";
                break;
            case EvaluationStyle::FUNCTIONAL:
                spec.evaluator.slug = "functional";
                spec.evaluator.solution_keys = styleConfig.solutionKeys();
                break;
        }
        spec.evaluator.tc_output_present = styleConfig.hasTcOutput();
        spec.evaluator.has_scorer = styleConfig.hasScorer();
        if (styleConfig.floatTolerance()) {
            spec.helpers["scorer"].additional_args = floatToleranceArgs(styleConfig.floatTolerance().value());
        }

        GradingConfig gradingConfig = testSpec_->TProblemSpec::buildGradingConfig();
        spec.limits.time_s = gradingConfig.timeLimit() ;
        spec.limits.memory_mb = gradingConfig.memoryLimit();

        IOFormat ioFormat = testSpec_->TProblemSpec::buildIOFormat();
        MultipleTestCasesConfig multipleTestCasesConfig = testSpec_->TProblemSpec::buildMultipleTestCasesConfig();

        auto testCaseDriver = new TestCaseDriver(
                new RawIOManipulator(),
                new IOManipulator(ioFormat),
                new Verifier(constraintSuite),
                multipleTestCasesConfig);

        TestSuite testSuite = testSpec_->buildTestSuite(spec.slug, constraintSuite.getDefinedSubtaskIds());
        SeedSetter* seedSetter = testSpec_->buildSeedSetter();

        auto specDriver = new SpecDriver(testCaseDriver, seedSetter, multipleTestCasesConfig, testSuite);

        return make_pair(spec, specDriver);
    }

private:
    // Same format the registry compare scorer takes (registry/helpers/scorer/compare/run).
    static string floatToleranceArgs(const FloatTolerance& tolerance) {
        ostringstream eps;
        eps << pow(10.0, -tolerance.k);

        ostringstream args;
        if (tolerance.mode != FloatToleranceMode::RELATIVE) {
            args << "float_absolute_tolerance " << eps.str();
        }
        if (tolerance.mode == FloatToleranceMode::BOTH) {
            args << " ";
        }
        if (tolerance.mode != FloatToleranceMode::ABSOLUTE) {
            args << "float_relative_tolerance " << eps.str();
        }
        return args.str();
    }
};

}

#pragma once

#include <ostream>
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

    // TODO (fushar): In 2.0, replace this with entry point
    virtual pair<SpecYaml, SpecDriver*> buildSpec() {
        SpecYaml spec;
        spec.slug = SlugParser::parse(specPath_);

        ConstraintSuite constraintSuite = testSpec_->TProblemSpec::buildConstraintSuite();
        if (constraintSuite.hasSubtasks()) {
            for (const Subtask& subtask : constraintSuite.constraints()) {
                if (subtask.id() != Subtask::MAIN_ID) {
                    SubtaskYaml subtaskYaml;
                    subtaskYaml.points = subtask.points();
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
        }
        spec.evaluator.has_tc_output = styleConfig.hasTcOutput();
        spec.evaluator.has_scorer = styleConfig.hasScorer();

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
};

}

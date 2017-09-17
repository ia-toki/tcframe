#pragma once

#include <string>

#include "GradingOptions.hpp"
#include "GraderLogger.hpp"
#include "tcframe/exception.hpp"
#include "tcframe/runner/evaluator.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"

using std::string;

namespace tcframe {

class TestCaseGrader {
private:
    Evaluator* evaluator_;
    GraderLogger* logger_;

public:
    virtual ~TestCaseGrader() = default;

    TestCaseGrader(Evaluator* evaluator, GraderLogger* logger)
            : evaluator_(evaluator)
            , logger_(logger) {}

    virtual Verdict grade(const TestCase& testCase, const GradingOptions& options) {
        logger_->logTestCaseIntroduction(testCase.name());

        EvaluationResult evaluationResult = evaluate(testCase, options);
        Verdict verdict = evaluationResult.verdict();

        logger_->logTestCaseVerdict(verdict);
        if (!(verdict.status() == VerdictStatus::tle())) {
            FormattedError error = ExecutionResults::asFormattedError(evaluationResult.executionResults());
            logger_->logError(&error);
        }

        return verdict;
    }

private:
    EvaluationResult evaluate(const TestCase& testCase, const GradingOptions& options) {
        string inputFilename = TestCasePathCreator::createInputPath(testCase.name(), options.outputDir());
        string outputFilename = TestCasePathCreator::createOutputPath(testCase.name(), options.outputDir());
        auto evaluationOptions = EvaluationOptionsBuilder()
                .setSolutionCommand(options.solutionCommand())
                .setTimeLimit(options.timeLimit())
                .setMemoryLimit(options.memoryLimit())
                .build();

        return evaluator_->evaluate(inputFilename, outputFilename, evaluationOptions);
    }
};

}

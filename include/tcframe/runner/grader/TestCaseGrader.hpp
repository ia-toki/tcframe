#pragma once

#include <string>

#include "GradingOptions.hpp"
#include "GraderLogger.hpp"
#include "tcframe/runner/evaluator.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::string;

namespace tcframe {

class TestCaseGrader {
private:
    Evaluator* evaluator_;
    GraderLogger* logger_;

public:
    virtual ~TestCaseGrader() {}

    TestCaseGrader(Evaluator* evaluator, GraderLogger* logger)
            : evaluator_(evaluator)
            , logger_(logger) {}

    virtual Verdict grade(const TestCase& testCase, const GradingOptions& options) {
        logger_->logTestCaseIntroduction(testCase.name());

        EvaluationResult evaluationResult = evaluate(testCase, options);
        Verdict verdict = evaluationResult.verdict();

        logger_->logTestCaseVerdict(verdict);
        if (!(verdict.status() == VerdictStatus::tle())) {
            logger_->logExecutionResults(evaluationResult.executionResults());
        }

        return verdict;
    }

private:
    EvaluationResult evaluate(const TestCase& testCase, const GradingOptions& options) {
        string inputFilename = options.outputDir() + "/" + testCase.name() + ".in";
        string outputFilename = options.outputDir() + "/" + testCase.name() + ".out";
        EvaluationOptions evaluationConfig = EvaluationOptionsBuilder()
                .setSolutionCommand(options.solutionCommand())
                .setTimeLimit(options.timeLimit())
                .setMemoryLimit(options.memoryLimit())
                .build();

        return evaluator_->evaluate(inputFilename, outputFilename, evaluationConfig);
    }
};

}

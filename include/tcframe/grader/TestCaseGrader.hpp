#pragma once

#include <string>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "tcframe/evaluator.hpp"
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

    virtual Verdict grade(const TestCase& testCase, const GraderConfig& config) {
        logger_->logTestCaseIntroduction(testCase.name());

        EvaluationResult evaluationResult = evaluate(testCase, config);
        Verdict verdict = evaluationResult.verdict();

        logger_->logTestCaseVerdict(verdict);
        if (!(verdict.status() == VerdictStatus::tle())) {
            logger_->logExecutionResults(evaluationResult.executionResults());
        }

        return verdict;
    }

private:
    EvaluationResult evaluate(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.name() + ".in";
        string outputFilename = config.outputDir() + "/" + testCase.name() + ".out";
        EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
                .setSolutionCommand(config.solutionCommand())
                .setTimeLimit(config.timeLimit())
                .setMemoryLimit(config.memoryLimit())
                .build();

        return evaluator_->evaluate(inputFilename, outputFilename, evaluatorConfig);
    }
};

}

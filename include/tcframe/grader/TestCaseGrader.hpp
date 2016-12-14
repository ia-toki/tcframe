#pragma once

#include <string>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/scorer.hpp"
#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

class TestCaseGrader {
private:
    Evaluator* evaluator_;
    Scorer* scorer_;
    GraderLogger* logger_;
public:
    virtual ~TestCaseGrader() {}

    TestCaseGrader(Evaluator* evaluator, Scorer* scorer, GraderLogger* logger)
            : evaluator_(evaluator)
            , scorer_(scorer)
            , logger_(logger) {}

    virtual Verdict grade(const TestCase& testCase, const GraderConfig& config) {
        logger_->logTestCaseIntroduction(testCase.id());

        optional<Verdict> verdict = evaluate(testCase, config);
        if (verdict) {
            return verdict.value();
        }
        return score(testCase, config);
    }

private:
    optional<Verdict> evaluate(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.id() + ".in";
        string evaluationFilename = "_evaluation.out";
        EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
                .setSolutionCommand(config.solutionCommand())
                .setTimeLimit(config.timeLimit())
                .setMemoryLimit(config.memoryLimit())
                .build();

        EvaluationResult result = evaluator_->evaluate(inputFilename, evaluationFilename, evaluatorConfig);

        if (result.verdict()) {
            Verdict verdict = result.verdict().value();
            logger_->logTestCaseVerdict(verdict);
            if (verdict == Verdict::rte()) {
                logger_->logSolutionExecutionFailure(result.executionResult());
            }
        }

        return result.verdict();
    }

    Verdict score(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.id() + ".in";
        string outputFilename = config.outputDir() + "/" + testCase.id() + ".out";
        string evaluationFilename = "_evaluation.out";

        ScoringResult result = scorer_->score(inputFilename, outputFilename, evaluationFilename);

        logger_->logTestCaseVerdict(result.verdict());
        if (!(result.verdict() == Verdict::ac())) {
            logger_->logTestCaseScoringMessage(result.message());
        }
        return result.verdict();
    }
};

}

#pragma once

#include <string>

#include "GraderConfig.hpp"
#include "GraderLogger.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/grade.hpp"
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

    virtual TestCaseGrade grade(const TestCase& testCase, const GraderConfig& config) {
        logger_->logTestCaseIntroduction(testCase.id());

        TestCaseGradeCreator gradeCreator;

        EvaluationResult evaluationResult = evaluate(testCase, config);
        gradeCreator.setEvaluationResult(evaluationResult);

        if (!evaluationResult.verdict()) {
            ScoringResult scoringResult = score(testCase, config);
            gradeCreator.setScoringResult(scoringResult);
        }

        TestCaseGrade grade = gradeCreator.create();

        logger_->logTestCaseGradeSummary(grade);
        logger_->logTestCaseGradeDetails(grade);

        return grade;
    }

private:
    EvaluationResult evaluate(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.id() + ".in";
        EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
                .setSolutionCommand(config.solutionCommand())
                .setTimeLimit(config.timeLimit())
                .setMemoryLimit(config.memoryLimit())
                .build();

        return evaluator_->evaluate(inputFilename, Evaluator::EVALUATION_FILENAME, evaluatorConfig);
    }

    ScoringResult score(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.id() + ".in";
        string outputFilename = config.outputDir() + "/" + testCase.id() + ".out";

        return scorer_->score(inputFilename, outputFilename, Evaluator::EVALUATION_FILENAME);
    }
};

}

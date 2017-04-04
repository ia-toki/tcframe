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
        logger_->logTestCaseIntroduction(testCase.name());

        TestCaseGradeCreator gradeCreator;

        EvaluationResult evaluationResult = evaluate(testCase, config);
        gradeCreator.setEvaluationResult(evaluationResult);

        if (!evaluationResult.verdict()) {
            ScoringResult scoringResult = score(testCase, config);
            gradeCreator.setScoringResult(scoringResult);
        }

        TestCaseGrade grade = gradeCreator.create();

        logger_->logTestCaseGradeSummary(grade);
        if (!(grade.verdict() == Verdict::tle())) {
            logger_->logTestCaseGradeDetails(grade);
        }

        return grade;
    }

private:
    EvaluationResult evaluate(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.name() + ".in";
        EvaluatorConfig evaluatorConfig = EvaluatorConfigBuilder()
                .setSolutionCommand(config.solutionCommand())
                .setTimeLimit(config.timeLimit())
                .setMemoryLimit(config.memoryLimit())
                .build();

        return evaluator_->evaluate(inputFilename, Evaluator::EVALUATION_FILENAME, evaluatorConfig);
    }

    ScoringResult score(const TestCase& testCase, const GraderConfig& config) {
        string inputFilename = config.outputDir() + "/" + testCase.name() + ".in";
        string outputFilename = config.outputDir() + "/" + testCase.name() + ".out";

        return scorer_->score(inputFilename, outputFilename, Evaluator::EVALUATION_FILENAME);
    }
};

}

#pragma once

#include "EvaluationResult.hpp"
#include "ScoringResult.hpp"
#include "TestCaseGrade.hpp"
#include "tcframe/util.hpp"

namespace tcframe {

class TestCaseGradeCreator {
private:
    optional<EvaluationResult> evaluationResult_;
    optional<ScoringResult> scoringResult_;

public:
    TestCaseGradeCreator& setEvaluationResult(EvaluationResult evaluationResult) {
        evaluationResult_ = optional<EvaluationResult>(evaluationResult);
        return *this;
    }

    TestCaseGradeCreator& setScoringResult(ScoringResult scoringResult) {
        scoringResult_ = optional<ScoringResult>(scoringResult);
        return *this;
    }

    TestCaseGrade create() {
        TestCaseGradeBuilder grade;

        if (evaluationResult_) {
            if (evaluationResult_.value().verdict()) {
                grade.setVerdict(evaluationResult_.value().verdict().value());
            }
            grade.setEvaluationExecutionResult(evaluationResult_.value().executionResult());
        }

        if (scoringResult_) {
            grade.setVerdict(scoringResult_.value().verdict());
            grade.setScoringExecutionResult(scoringResult_.value().executionResult());
            grade.setPrivateScoringInfo(scoringResult_.value().privateInfo());
        }

        return grade.build();
    }
};

}

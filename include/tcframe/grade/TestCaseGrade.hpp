#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct TestCaseGrade {
    friend class TestCaseGradeBuilder;

private:
    Verdict verdict_;
    optional<double> score_;

    optional<ExecutionResult> evaluationExecutionResult_;
    optional<ExecutionResult> scoringExecutionResult_;
    optional<string> privateScoringInfo_;

public:
    const Verdict& verdict() const {
        return verdict_;
    }

    const optional<double>& score() const {
        return score_;
    }

    optional<ExecutionResult> evaluationExecutionResult() const {
        return evaluationExecutionResult_;
    }

    optional<ExecutionResult> scoringExecutionResult() const {
        return scoringExecutionResult_;
    }

    const optional<string>& privateScoringInfo() const {
        return privateScoringInfo_;
    }

    bool operator==(const TestCaseGrade& o) const {
        return tie(
                verdict_,
                score_,
                evaluationExecutionResult_,
                scoringExecutionResult_,
                privateScoringInfo_) == tie(
                o.verdict_,
                o.score_,
                o.evaluationExecutionResult_,
                o.scoringExecutionResult_,
                o.privateScoringInfo_);
    }
};

class TestCaseGradeBuilder {
private:
    TestCaseGrade subject_;

public:
    TestCaseGradeBuilder& setVerdict(Verdict verdict) {
        subject_.verdict_ = verdict;
        return *this;
    }

    TestCaseGradeBuilder& setScore(double score) {
        subject_.score_ = optional<double>(score);
        return *this;
    }

    TestCaseGradeBuilder& setEvaluationExecutionResult(ExecutionResult evaluationExecutionResult) {
        return setEvaluationExecutionResult(optional<ExecutionResult>(evaluationExecutionResult));
    }

    TestCaseGradeBuilder& setEvaluationExecutionResult(optional<ExecutionResult> evaluationExecutionResult) {
        subject_.evaluationExecutionResult_ = evaluationExecutionResult;
        return *this;
    }

    TestCaseGradeBuilder& setScoringExecutionResult(ExecutionResult scoringExecutionResult) {
        return setScoringExecutionResult(optional<ExecutionResult>(scoringExecutionResult));
    }

    TestCaseGradeBuilder& setScoringExecutionResult(optional<ExecutionResult> scoringExecutionResult) {
        subject_.scoringExecutionResult_ = scoringExecutionResult;
        return *this;
    }

    TestCaseGradeBuilder& setPrivateScoringInfo(string privateScoringInfo) {
        return setPrivateScoringInfo(optional<string>(privateScoringInfo));
    }

    TestCaseGradeBuilder& setPrivateScoringInfo(optional<string> privateScoringInfo) {
        subject_.privateScoringInfo_ = privateScoringInfo;
        return *this;
    }

    TestCaseGrade build() {
        return move(subject_);
    }
};

}

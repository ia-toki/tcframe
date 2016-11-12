#pragma once

#include "Evaluator.hpp"
#include "GraderLogger.hpp"
#include "Scorer.hpp"

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

        optional<Verdict> verdict = evaluator_->evaluate(testCase, config);
        if (verdict) {
            return verdict.value();
        }
        return scorer_->score(testCase, config);
    }
};

}

#pragma once

#include "Evaluator.hpp"
#include "Scorer.hpp"
#include "SubmitterLogger.hpp"

namespace tcframe {

class TestCaseSubmitter {
private:
    Evaluator* evaluator_;
    Scorer* scorer_;
    SubmitterLogger* logger_;
public:
    virtual ~TestCaseSubmitter() {}

    TestCaseSubmitter(Evaluator* evaluator, Scorer* scorer, SubmitterLogger* logger)
            : evaluator_(evaluator)
            , scorer_(scorer)
            , logger_(logger) {}

    virtual Verdict submit(const TestCase &testCase, const SubmitterConfig &config) {
        logger_->logTestCaseIntroduction(testCase.id());

        optional<Verdict> verdict = evaluator_->evaluate(testCase, config);
        if (verdict) {
            return verdict.value();
        }
        return scorer_->score(testCase, config);
    }
};

}

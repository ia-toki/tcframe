#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/scorer/Scorer.hpp"

namespace tcframe {

class MockScorer : public Scorer {
public:
    MOCK_METHOD3(score, ScoringResult(const string&, const string&, const string&));
};

}

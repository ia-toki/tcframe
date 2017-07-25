#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/Evaluator.hpp"

namespace tcframe {

class MockEvaluator : public Evaluator {
public:
    MOCK_METHOD3(evaluate, EvaluationResult(const string&, const string&, const EvaluationOptions&));
    MOCK_METHOD3(generate, GenerationResult(const string&, const string&, const EvaluationOptions&));
    MOCK_METHOD2(score, ScoringResult(const string&, const string&));
};

}

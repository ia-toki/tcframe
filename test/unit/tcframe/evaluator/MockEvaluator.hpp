#pragma once

#include "gmock/gmock.h"

#include "tcframe/evaluator/Evaluator.hpp"
#include "tcframe/evaluator/EvaluatorFactory.hpp"

namespace tcframe {

class MockEvaluator : public Evaluator {
public:
    MOCK_METHOD3(evaluate, EvaluationResult(const string&, const string&, const EvaluatorConfig&));
    MOCK_METHOD3(generate, GenerationResult(const string&, const string&, const EvaluatorConfig&));
    MOCK_METHOD2(score, ScoringResult(const string&, const string&));
};

class MockEvaluatorFactory : public EvaluatorFactory {
public:
    MOCK_METHOD2(createBatch, Evaluator*(OperatingSystem*, const optional<string>&));
};

}

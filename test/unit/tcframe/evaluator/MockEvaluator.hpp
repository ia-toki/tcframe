#pragma once

#include "gmock/gmock.h"

#include "tcframe/evaluator/Evaluator.hpp"

namespace tcframe {

class MockEvaluator : public Evaluator {
public:
    MOCK_METHOD3(evaluate, EvaluationResult(const string&, const string&, const EvaluatorConfig&));
};

}

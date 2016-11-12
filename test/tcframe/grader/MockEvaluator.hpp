#pragma once

#include "gmock/gmock.h"

#include "tcframe/grader/Evaluator.hpp"

namespace tcframe {

class MockEvaluator : public Evaluator {
public:
    MOCK_METHOD2(evaluate, optional<Verdict>(const TestCase&, const GraderConfig&));
};

}

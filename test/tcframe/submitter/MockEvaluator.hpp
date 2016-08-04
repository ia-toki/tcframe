#pragma once

#include "gmock/gmock.h"

#include "tcframe/submitter/Evaluator.hpp"

namespace tcframe {

class MockEvaluator : public Evaluator {
public:
    MOCK_METHOD2(evaluate, optional<Verdict>(const TestCase&, const SubmitterConfig&));
};

}

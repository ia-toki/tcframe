#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/EvaluatorRegistry.hpp"

namespace tcframe {

class MockEvaluatorRegistry : public EvaluatorRegistry {
public:
    MockEvaluatorRegistry()
            : EvaluatorRegistry(nullptr) {}

    MOCK_METHOD4(get, Evaluator*(const string&, OperatingSystem*, const map<string, string>&, const string&));
};

}

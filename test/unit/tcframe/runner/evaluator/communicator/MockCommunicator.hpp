#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/evaluator/communicator/Communicator.hpp"

namespace tcframe {

class MockCommunicator : public Communicator {
public:
    MockCommunicator()
            : Communicator(nullptr, nullptr, "") {}

    MOCK_METHOD2(communicate, CommunicationResult(const string&, const EvaluationOptions&));
};

}

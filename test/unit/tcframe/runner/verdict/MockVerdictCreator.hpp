#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/verdict/VerdictCreator.hpp"

namespace tcframe {

class MockVerdictCreator : public VerdictCreator {
public:
    MOCK_METHOD1(fromStream, Verdict(istream*));
    MOCK_METHOD1(fromExecutionResult, optional<Verdict>(const ExecutionResult&));
};

}

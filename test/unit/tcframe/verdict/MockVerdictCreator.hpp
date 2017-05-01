#pragma once

#include "gmock/gmock.h"

#include "tcframe/verdict/VerdictCreator.hpp"

namespace tcframe {

class MockVerdictCreator : public VerdictCreator {
public:
    MOCK_METHOD1(fromStream, Verdict(istream*));
    MOCK_METHOD1(fromExecutionResult, optional<Verdict>(const ExecutionResult&));
};

}

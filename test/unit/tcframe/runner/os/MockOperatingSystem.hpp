#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/os/OperatingSystem.hpp"

namespace tcframe {

class MockOperatingSystem : public OperatingSystem {
public:
    MOCK_METHOD1(openForReading, istream*(const string&));
    MOCK_METHOD1(openForWriting, ostream*(const string&));
    MOCK_METHOD1(closeOpenedStream, void(ios*));
    MOCK_METHOD1(forceMakeDir, void(const string&));
    MOCK_METHOD1(removeFile, void(const string&));
    MOCK_METHOD1(execute, ExecutionResult(const ExecutionRequest&));
};

}

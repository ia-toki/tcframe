#pragma once

#include "gmock/gmock.h"

#include "tcframe/os/OperatingSystem.hpp"

namespace tcframe {

class MockOperatingSystem : public OperatingSystem {
public:
    MOCK_METHOD1(openForReading, istream*(const string&));
    MOCK_METHOD1(openForWriting, ostream*(const string&));
    MOCK_METHOD1(closeOpenedWritingStream, void(ostream*));
    MOCK_METHOD1(forceMakeDir, void(const string&));
    MOCK_METHOD1(removeFile, void(const string&));
    MOCK_METHOD1(execute, ExecutionResult(const ExecutionRequest&));
    MOCK_METHOD2(combineMultipleTestCases, void(const string&, int));
};

}

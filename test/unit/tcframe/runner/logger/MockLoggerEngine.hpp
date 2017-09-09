#pragma once

#include "gmock/gmock.h"

#include "tcframe/runner/logger/LoggerEngine.hpp"

namespace tcframe {

class MockLoggerEngine : public LoggerEngine {
public:
    MOCK_METHOD1(logHeading, void(const string&));
    MOCK_METHOD2(logParagraph, void(int, const string&));
    MOCK_METHOD2(logHangingParagraph, void(int, const string&));
    MOCK_METHOD2(logListItem1, void(int, const string&));
    MOCK_METHOD2(logListItem2, void(int, const string&));
};

}

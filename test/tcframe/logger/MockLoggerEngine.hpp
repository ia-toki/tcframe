#pragma once

#include "gmock/gmock.h"

#include "tcframe/logger/LoggerEngine.hpp"

namespace tcframe {

class MockLoggerEngine : public LoggerEngine {
public:
    MOCK_METHOD1(logHeading, void(string));
    MOCK_METHOD2(logParagraph, void(int, string));
    MOCK_METHOD2(logHangingParagraph, void(int, string));
    MOCK_METHOD2(logListItem1, void(int, string));
    MOCK_METHOD2(logListItem2, void(int, string));
};

}

#pragma once

#include "gmock/gmock.h"

#include "tcframe/io/WhitespaceManipulator.hpp"

namespace tcframe {

class MockWhitespaceManipulator : public WhitespaceManipulator {
public:
    MOCK_METHOD2(parseSpace, void(istream*, const string&));
    MOCK_METHOD2(parseNewline, void(istream*, const string&));
    MOCK_METHOD1(ensureEof, void(istream*));
    MOCK_METHOD1(printSpace, void(ostream*));
    MOCK_METHOD1(printNewline, void(ostream*));
};

}

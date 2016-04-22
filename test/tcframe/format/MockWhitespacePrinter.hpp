#pragma once

#include "gmock/gmock.h"

#include "tcframe/format/WhitespacePrinter.hpp"

namespace tcframe {

class MockWhitespacePrinter : public WhitespacePrinter {
public:
    MOCK_METHOD1(printSpace, void(ostream*));
    MOCK_METHOD1(printNewline, void(ostream*));
};

}

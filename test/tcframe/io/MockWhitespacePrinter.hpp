#pragma once

#include "gmock/gmock.h"

#include <ostream>

#include "tcframe/io/WhitespacePrinter.hpp"

using std::ostream;

namespace tcframe {

class MockWhitespacePrinter : public WhitespacePrinter {
public:
    MOCK_METHOD1(printSpace, void(ostream*));
    MOCK_METHOD1(printNewline, void(ostream*));
};

}

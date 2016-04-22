#pragma once

#include "gmock/gmock.h"

#include "tcframe/format/LineIOSegmentPrinter.hpp"

namespace tcframe {

class MockLineIOSegmentPrinter : public LineIOSegmentPrinter {
public:
    MockLineIOSegmentPrinter()
            : LineIOSegmentPrinter(nullptr)
    {}

    MOCK_METHOD2(print, void(LineIOSegment*, ostream*));
};

}

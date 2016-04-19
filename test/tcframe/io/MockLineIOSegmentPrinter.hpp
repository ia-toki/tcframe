#pragma once

#include "gmock/gmock.h"

#include <ostream>

#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/io/LineIOSegmentPrinter.hpp"

using std::ostream;

namespace tcframe {

class MockLineIOSegmentPrinter : public LineIOSegmentPrinter {
public:
    MockLineIOSegmentPrinter()
            : LineIOSegmentPrinter(nullptr)
    {}

    MOCK_METHOD2(print, void(LineIOSegment*, ostream*));
};

}

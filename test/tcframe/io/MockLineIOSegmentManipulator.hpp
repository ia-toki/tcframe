#pragma once

#include "gmock/gmock.h"

#include "tcframe/io/LineIOSegmentManipulator.hpp"

namespace tcframe {

class MockLineIOSegmentManipulator : public LineIOSegmentManipulator {
public:
    MockLineIOSegmentManipulator()
            : LineIOSegmentManipulator(nullptr)
    {}

    MOCK_METHOD2(print, void(LineIOSegment*, ostream*));
};

}

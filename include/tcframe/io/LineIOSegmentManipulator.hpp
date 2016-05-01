#pragma once

#include <iostream>

#include "LineIOSegment.hpp"
#include "WhitespaceManipulator.hpp"
#include "tcframe/variable.hpp"

using std::istream;
using std::ostream;

namespace tcframe {

class LineIOSegmentManipulator {
private:
    WhitespaceManipulator* whitespaceManipulator_;

public:
    virtual ~LineIOSegmentManipulator() {}

    LineIOSegmentManipulator(WhitespaceManipulator* whitespaceManipulator)
            : whitespaceManipulator_(whitespaceManipulator)
    {}

    virtual void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!first) {
                whitespaceManipulator_->printSpace(out);
            }
            first = false;

            if (segmentVariable.variable()->type() == VariableType::SCALAR) {
                auto scalar = (Scalar*) segmentVariable.variable();
                scalar->printTo(out);
            }
        }
        whitespaceManipulator_->printNewline(out);
    }
};

}

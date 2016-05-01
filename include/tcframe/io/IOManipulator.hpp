#pragma once

#include <iostream>
#include <ostream>
#include <vector>

#include "LineIOSegmentManipulator.hpp"
#include "tcframe/io.hpp"

using std::endl;
using std::ostream;
using std::vector;

namespace tcframe {

class IOManipulator {
private:
    LineIOSegmentManipulator* lineIOSegmentManipulator_;
    IOFormat ioFormat_;

public:
    virtual ~IOManipulator() {}

    IOManipulator(
            LineIOSegmentManipulator* lineIOSegmentManipulator,
            const IOFormat& ioFormat)
            : lineIOSegmentManipulator_(lineIOSegmentManipulator)
            , ioFormat_(ioFormat)
    {}

    virtual void printInput(ostream* out) {
        print(ioFormat_.inputFormat(), out);
    }

private:
    void print(const vector<IOSegment*>& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::LINE) {
                lineIOSegmentManipulator_->print((LineIOSegment*) segment, out);
            }
        }
    }
};

}

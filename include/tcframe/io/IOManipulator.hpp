#pragma once

#include <iostream>
#include <vector>

#include "IOFormat.hpp"
#include "LineIOSegmentManipulator.hpp"
#include "tcframe/variable.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::vector;

namespace tcframe {

class IOManipulator {
private:
    IOFormat ioFormat_;

public:
    virtual ~IOManipulator() {}

    IOManipulator(const IOFormat& ioFormat)
            : ioFormat_(ioFormat)
    {}

    virtual void printInput(ostream* out) {
        print(ioFormat_.inputFormat(), out);
    }

    virtual void parseInput(istream* in) {
        parse(ioFormat_.inputFormat(), in);
    }

private:
    void print(const vector<IOSegment*>& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::LINE) {
                LineIOSegmentManipulator::print((LineIOSegment*) segment, out);
            }
        }
    }

    void parse(const vector<IOSegment*>& segments, istream* in) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::LINE) {
                LineIOSegmentManipulator::parse((LineIOSegment*) segment, in);
            }
        }
        WhitespaceManipulator::ensureEof(in);
    }
};

}

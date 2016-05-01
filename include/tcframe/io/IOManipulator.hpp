#pragma once

#include <iostream>
#include <vector>

#include "IOFormat.hpp"
#include "LineIOSegmentManipulator.hpp"
#include "WhitespaceManipulator.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::vector;

namespace tcframe {

class IOManipulator {
private:
    LineIOSegmentManipulator* lineIOSegmentManipulator_;
    WhitespaceManipulator* whitespaceManipulator_;
    IOFormat ioFormat_;

public:
    virtual ~IOManipulator() {}

    IOManipulator(
            LineIOSegmentManipulator* lineIOSegmentManipulator,
            WhitespaceManipulator* whitespaceManipulator,
            const IOFormat& ioFormat)
            : lineIOSegmentManipulator_(lineIOSegmentManipulator)
            , whitespaceManipulator_(whitespaceManipulator)
            , ioFormat_(ioFormat)
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
                lineIOSegmentManipulator_->print((LineIOSegment*) segment, out);
            }
        }
    }

    void parse(const vector<IOSegment*>& segments, istream* in) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::LINE) {
                lineIOSegmentManipulator_->parse((LineIOSegment*) segment, in);
            }
        }
        whitespaceManipulator_->ensureEof(in);
    }
};

}

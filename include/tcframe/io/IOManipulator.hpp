#pragma once

#include <iostream>
#include <vector>

#include "GridIOSegmentManipulator.hpp"
#include "IOFormat.hpp"
#include "LineIOSegmentManipulator.hpp"
#include "LinesIOSegmentManipulator.hpp"
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

    virtual void parseOutput(istream* in) {
        if (!ioFormat_.outputFormat().empty()) {
            parse(ioFormat_.outputFormat(), in);
        }
    }

private:
    void print(const vector<IOSegment*>& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                GridIOSegmentManipulator::print((GridIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINE) {
                LineIOSegmentManipulator::print((LineIOSegment*) segment, out);
            } else if (segment->type() == IOSegmentType::LINES) {
                LinesIOSegmentManipulator::print((LinesIOSegment*) segment, out);
            }
        }
    }

    void parse(const vector<IOSegment*>& segments, istream* in) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::GRID) {
                GridIOSegmentManipulator::parse((GridIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINE) {
                LineIOSegmentManipulator::parse((LineIOSegment*) segment, in);
            } else if (segment->type() == IOSegmentType::LINES) {
                LinesIOSegmentManipulator::parse((LinesIOSegment*) segment, in);
            }
        }
        WhitespaceManipulator::ensureEof(in);
    }
};

}

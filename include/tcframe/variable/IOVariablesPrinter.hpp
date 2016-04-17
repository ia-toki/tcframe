#pragma once

#include <iostream>
#include <ostream>
#include <vector>

#include "tcframe/io/IOFormat.hpp"
#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/io/LineIOSegmentPrinter.hpp"

using std::endl;
using std::ostream;
using std::vector;

namespace tcframe {

class IOVariablesPrinter {
private:
    LineIOSegmentPrinter* lineIOSegmentPrinter_;
    IOFormat ioFormat_;

public:
    virtual ~IOVariablesPrinter() {}

    IOVariablesPrinter(
            LineIOSegmentPrinter* lineIOSegmentPrinter,
            const IOFormat& ioFormat)
            : lineIOSegmentPrinter_(lineIOSegmentPrinter)
            , ioFormat_(ioFormat)
    {}

    void printInput(ostream* out) {
        print(ioFormat_.inputFormat(), out);
    }

private:
    void print(const vector<IOSegment*>& segments, ostream* out) {
        for (IOSegment* segment : segments) {
            if (segment->type() == IOSegmentType::LINE) {
                lineIOSegmentPrinter_->print((LineIOSegment*) segment, out);
            }
        }
    }
};

}

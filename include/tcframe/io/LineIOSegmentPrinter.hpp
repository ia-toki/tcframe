#pragma once

#include <ostream>

#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/io/LineIOSegmentScalarVariable.hpp"
#include "tcframe/io/WhitespacePrinter.hpp"

using std::endl;
using std::ostream;

namespace tcframe {

class LineIOSegmentPrinter {
private:
    WhitespacePrinter* whitespacePrinter_;

public:
    virtual ~LineIOSegmentPrinter() {}

    LineIOSegmentPrinter(WhitespacePrinter* whitespacePrinter)
            : whitespacePrinter_(whitespacePrinter)
    {}

    virtual void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (LineIOSegmentVariable* segmentVariable : segment->variables()) {
            if (!first) {
                whitespacePrinter_->printSpace(out);
            }
            first = false;

            if (segmentVariable->type() == LineIOSegmentVariableType:: SCALAR) {
                auto scalarVariable = (LineIOSegmentScalarVariable*) segmentVariable;
                scalarVariable->variable()->printTo(out);
            }
        }
        whitespacePrinter_->printNewline(out);
    }
};

}

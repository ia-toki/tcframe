#pragma once

#include <ostream>

#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/io/LineIOSegmentVariable.hpp"
#include "tcframe/io/LineIOSegmentScalarVariable.hpp"

using std::endl;
using std::ostream;

namespace tcframe {

class LineIOSegmentPrinter {
public:
    void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (LineIOSegmentVariable* segmentVariable : segment->variables()) {
            if (!first) {
                *out << " ";
            }
            first = false;

            if (segmentVariable->type() == LineIOSegmentVariableType:: SCALAR) {
                auto scalarVariable = (LineIOSegmentScalarVariable*) segmentVariable;
                scalarVariable->variable()->printTo(*out);
            }
        }
        *out << endl;
    }
};

}

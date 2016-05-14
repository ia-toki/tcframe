#pragma once

#include <iostream>

#include "LineIOSegment.hpp"
#include "tcframe/util/WhitespaceUtils.hpp"
#include "tcframe/variable.hpp"

using std::endl;
using std::istream;
using std::ostream;

namespace tcframe {

class LineIOSegmentManipulator {
public:
    static void parse(LineIOSegment* segment, istream* in) {
        string lastVariableName;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!lastVariableName.empty()) {
                WhitespaceUtils::parseSpace(in, lastVariableName);
            }

            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                parseScalar((Scalar*) variable, in);
            }

            lastVariableName = variable->name();
        }
        WhitespaceUtils::parseNewline(in, lastVariableName);
    }

    static void print(LineIOSegment* segment, ostream* out) {
        bool first = true;
        for (const LineIOSegmentVariable& segmentVariable : segment->variables()) {
            if (!first) {
                *out << ' ';
            }
            first = false;

            Variable* variable = segmentVariable.variable();
            int size = segmentVariable.size();

            if (variable->type() == VariableType::SCALAR) {
                printScalar((Scalar*) variable, out);
            }
        }
        *out << endl;
    }

private:
    static void parseScalar(Scalar* scalar, istream* in) {
        scalar->parseFrom(in);
    }

    static void printScalar(Scalar* scalar, ostream* out) {
        scalar->printTo(out);
    }
};

}

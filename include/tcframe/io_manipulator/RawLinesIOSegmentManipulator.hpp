#pragma once

#include <iostream>
#include <stdexcept>

#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;
using std::runtime_error;

namespace tcframe {

class RawLinesIOSegmentManipulator {
public:
    static string parse(RawLinesIOSegment* segment, istream* in) {
        string lastVariableName;

        Vector* variable = segment->variable();
        for (int j = 0; j != *segment->size(); j++) {
            if (*segment->size() == -1 && WhitespaceManipulator::isEof(in)) {
                break;
            }

            variable->parseAndAddElementFrom(in);
            lastVariableName = TokenFormatter::formatVectorElement(variable->name(), j);

            WhitespaceManipulator::parseNewline(in, lastVariableName);
        }

        return lastVariableName;
    }

    static void print(RawLinesIOSegment* segment, ostream* out) {
        checkVectorSize(segment);
        Vector* variable = segment->variable();
        for (int j = 0; j < variable->size(); j++) {
            variable->printElementTo(j, out);
            *out << endl;
        }
    }

private:
    static void checkVectorSize(RawLinesIOSegment* segment) {
        Vector* variable = segment->variable();
        if (*segment->size() != -1 && *segment->size() != variable->size()) {
            throw runtime_error(
                    "Number of elements of " + TokenFormatter::formatVariable(variable->name())
                    + " unsatisfied. Expected: " + StringUtils::toString(*segment->size())
                    + ", actual: " + StringUtils::toString(variable->size()));
        }
    }
};

}

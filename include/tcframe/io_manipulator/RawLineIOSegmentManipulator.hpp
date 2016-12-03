#pragma once

#include <iostream>

#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::ostream;

namespace tcframe {

class RawLineIOSegmentManipulator {
public:
    static string parse(RawLineIOSegment* segment, istream* in) {
        string lastVariableName = TokenFormatter::formatVariable(segment->variable()->name());
        segment->variable()->parseFrom(in);
        WhitespaceManipulator::parseNewline(in, lastVariableName);

        return lastVariableName;
    }

    static void print(RawLineIOSegment* segment, ostream* out) {
        segment->variable()->printTo(out);
        *out << endl;
    }
};

}

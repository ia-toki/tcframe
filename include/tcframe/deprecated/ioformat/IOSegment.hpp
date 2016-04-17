#pragma once

#include "tcframe/deprecated/iovariable/ParsingException.hpp"

#include <istream>
#include <ostream>
#include <string>

using std::istream;
using std::ostream;
using std::string;

namespace tcframe_old {

class IOSegment {
public:
    virtual ~IOSegment() { }

    virtual void printTo(ostream& out) = 0;

    virtual void parseFrom(istream& in) = 0;

protected:
    static void parseSpaceFrom(istream& in, string lastRealName) {
        if (in.get() != ' ') {
            throw ParsingException("Expected: <space> after variable `" + lastRealName + "`");
        }
    }

    static void parseNewLineFrom(istream& in, string lastRealName) {
        if (in.get() != '\n') {
            if (lastRealName == "") {
                throw ParsingException("Expected: <space> after empty line");
            } else {
                throw ParsingException("Expected: <new line> after variable `" + lastRealName + "`");
            }
        }
    }
};

}

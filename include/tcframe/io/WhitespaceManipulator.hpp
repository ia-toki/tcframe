#pragma once

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;

namespace tcframe {

class WhitespaceManipulator {
public:
    virtual ~WhitespaceManipulator() {}

    virtual void parseSpace(istream* in, const string& context) {
        if (in->get() != ' ') {
            throw runtime_error("Expected: <space> after '" + context + "'");
        }
    }

    virtual void parseNewline(istream* in, const string& context) {
        if (in->get() != '\n') {
            throw runtime_error("Expected: <new line> after '" + context + "'");
        }
    }
    
    virtual void ensureEof(istream* in) {
        if (in->peek() != char_traits<char>::eof()) {
            throw runtime_error("Expected: <EOF>");
        }
    }

    virtual void printSpace(ostream* out) {
        *out << ' ';
    }

    virtual void printNewline(ostream* out) {
        *out << '\n';
    }
};

}

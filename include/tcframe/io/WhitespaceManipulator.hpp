#pragma once

#include <iostream>

using std::ostream;

namespace tcframe {

class WhitespaceManipulator {
public:
    virtual ~WhitespaceManipulator() {}

    virtual void printSpace(ostream* out) {
        *out << ' ';
    }

    virtual void printNewline(ostream* out) {
        *out << '\n';
    }
};

}

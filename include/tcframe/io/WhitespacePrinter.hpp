#pragma once

#include <ostream>

using std::ostream;

namespace tcframe {

class WhitespacePrinter {
public:
    virtual ~WhitespacePrinter() {}

    virtual void printSpace(ostream* out) {
        *out << ' ';
    }

    virtual void printNewline(ostream* out) {
        *out << '\n';
    }
};

}

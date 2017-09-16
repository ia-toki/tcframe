#pragma once

#include <ostream>
#include <string>

using std::endl;
using std::ostream;
using std::string;

namespace tcframe {

class RawIOManipulator {
public:
    virtual ~RawIOManipulator() = default;

    virtual void print(ostream* out, const string& data) {
        *out << data;
    }

    virtual void printLine(ostream* out, const string& data) {
        *out << data << endl;
    }
};

}

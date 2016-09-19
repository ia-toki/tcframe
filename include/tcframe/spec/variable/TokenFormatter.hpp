#pragma once

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe {

class TokenFormatter {
public:
    TokenFormatter() = delete;

    static string formatVariable(const string& name) {
        return "'" + name + "'";
    }

    static string formatVectorElement(const string& name, int index) {
        ostringstream sout;
        sout << "'" << name << "[" << index << "]'";
        return sout.str();
    }

    static string formatMatrixElement(const string& name, int rowIndex, int colIndex) {
        ostringstream sout;
        sout << "'" << name << "[" << rowIndex << "][" << colIndex << "]'";
        return sout.str();
    }
};

}

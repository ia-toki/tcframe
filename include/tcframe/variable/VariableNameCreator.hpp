#pragma once

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe {

class VariableNameCreator {
public:
    VariableNameCreator() = delete;

    static string createName(const string& name) {
        return "'" + name + "'";
    }

    static string createVectorElementName(const string& name, int index) {
        ostringstream sout;
        sout << "'" << name << "[" << index << "]'";
        return sout.str();
    }
};

}

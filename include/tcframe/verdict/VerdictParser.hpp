#pragma once

#include <istream>
#include <stdexcept>
#include <string>

#include "Verdict.hpp"

using std::getline;
using std::istream;
using std::runtime_error;
using std::string;

namespace tcframe {

class VerdictParser {
public:
    VerdictParser() = delete;

    static Verdict parse(istream* in) {
        string verdictString;
        if (!getline(*in, verdictString)) {
            throw runtime_error("Expected: <verdict> on the first line");
        }

        if (verdictString == "AC") {
            return Verdict::ac();
        } else if (verdictString == "WA") {
            return Verdict::wa();
        } else {
            throw runtime_error("Unknown verdict: " + verdictString);
        }
    }
};

}

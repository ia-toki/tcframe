#pragma once

#include <istream>
#include <stdexcept>
#include <string>

#include "tcframe/grade.hpp"

using std::getline;
using std::istream;
using std::runtime_error;
using std::string;

namespace tcframe {

class ScoringResultParser {
public:
    ScoringResultParser() = delete;

    static ScoringResult parse(istream* in) {
        ScoringResultBuilder builder;

        string verdictString;
        if (!getline(*in, verdictString)) {
            builder.setVerdict(Verdict::err());
            builder.setPrivateInfo("Expected: <verdict> on the first line");
            return builder.build();
        }

        if (verdictString == "AC") {
            builder.setVerdict(Verdict::ac());
        } else if (verdictString == "WA") {
            builder.setVerdict(Verdict::wa());
        } else {
            builder.setVerdict(Verdict::err());
            builder.setPrivateInfo("Unknown verdict: " + verdictString);
        }

        return builder.build();
    }
};

}

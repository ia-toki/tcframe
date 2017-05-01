#pragma once

#include <istream>
#include <stdexcept>
#include <string>

#include "Verdict.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"

using std::getline;
using std::istream;
using std::runtime_error;
using std::string;

namespace tcframe {

class VerdictCreator {
public:
    virtual ~VerdictCreator() {}

    virtual Verdict fromStream(istream* in) {
        Verdict builder;

        string verdictString;
        if (!getline(*in, verdictString)) {
            throw runtime_error("Expected: <verdict> on the first line");
        }

        if (verdictString == "AC") {
            return Verdict(VerdictStatus::ac());
        } else if (verdictString == "WA") {
            return Verdict(VerdictStatus::wa());
        }

        throw runtime_error("Unknown verdict: " + verdictString);
    }

    virtual optional<Verdict> fromExecutionResult(const ExecutionResult& executionResult) {
        if (executionResult.exceededCpuLimits()) {
            return optional<Verdict>(Verdict(VerdictStatus::tle()));
        } else if (!executionResult.isSuccessful()) {
            return optional<Verdict>(Verdict(VerdictStatus::rte()));
        }
        return optional<Verdict>();
    }
};

}

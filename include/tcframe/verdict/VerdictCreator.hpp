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
        } else if (verdictString == "OK") {
            string secondLine;
            if (!getline(*in, secondLine)) {
                throw runtime_error("Expected: <points> on the second line");
            }
            string pointsString = StringUtils::split(secondLine, ' ')[0];
            optional<double> points = StringUtils::toNumber<double>(pointsString);
            if (points) {
                return Verdict(VerdictStatus::ok(), points.value());
            }
            throw runtime_error("Unknown points format: " + pointsString);
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

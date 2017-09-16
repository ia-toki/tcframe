#pragma once

#include <istream>
#include <stdexcept>
#include <string>
#include <sys/signal.h>

#include "Verdict.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/util.hpp"

using std::getline;
using std::istream;
using std::runtime_error;
using std::string;

namespace tcframe {

class VerdictCreator {
public:
    virtual ~VerdictCreator() = default;

    virtual Verdict fromStream(istream* in) {
        Verdict builder;

        string statusString;
        if (!getline(*in, statusString)) {
            throw runtime_error("Expected: <status> on the first line");
        }

        if (statusString == "AC") {
            return Verdict(VerdictStatus::ac());
        } else if (statusString == "WA") {
            return Verdict(VerdictStatus::wa());
        } else if (statusString == "OK") {
            string secondLine;
            if (!getline(*in, secondLine)) {
                throw runtime_error("Expected: <points> on the second line");
            }
            string pointsString = StringUtils::split(secondLine, ' ')[0];
            optional<double> points = StringUtils::toNumber<double>(pointsString);
            if (points) {
                return {VerdictStatus::ok(), points.value()};
            }
            throw runtime_error("Unknown points format: " + pointsString);
        }

        throw runtime_error("Unknown status: " + statusString);
    }

    virtual optional<Verdict> fromExecutionResult(const ExecutionResult& executionResult) {
        if (executionResult.exitSignal() == optional<int>(SIGXCPU)) {
            return optional<Verdict>(Verdict(VerdictStatus::tle()));
        } else if (!executionResult.isSuccessful()) {
            return optional<Verdict>(Verdict(VerdictStatus::rte()));
        }
        return {};
    }
};

}

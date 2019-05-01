#pragma once

#include <istream>
#include <stdexcept>
#include <string>
#include <sys/signal.h>

#include "TestCaseVerdict.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/util.hpp"

using std::getline;
using std::istream;
using std::runtime_error;
using std::string;

namespace tcframe {

class TestCaseVerdictParser {
public:
    virtual ~TestCaseVerdictParser() = default;

    virtual TestCaseVerdict parseStream(istream* in) {
        string verdictCode;
        if (!getline(*in, verdictCode)) {
            throw runtime_error("Expected: <verdict> on the first line");
        }

        if (verdictCode == "AC") {
            return TestCaseVerdict(Verdict::ac());
        } else if (verdictCode == "WA") {
            return TestCaseVerdict(Verdict::wa());
        } else if (verdictCode == "OK") {
            string secondLine;
            if (!getline(*in, secondLine)) {
                throw runtime_error("Expected: <points> on the second line");
            }
            string pointsString = StringUtils::split(secondLine, ' ')[0];
            optional<double> points = StringUtils::toNumber<double>(pointsString);
            if (points) {
                return {Verdict::ok(), points.value()};
            }
            throw runtime_error("Unknown points format: " + pointsString);
        }

        throw runtime_error("Unknown verdict: " + verdictCode);
    }

    virtual optional<TestCaseVerdict> parseExecutionResult(const ExecutionResult& executionResult) {
        if (executionResult.exitSignal() == optional<int>(SIGXCPU)) {
            return optional<TestCaseVerdict>(TestCaseVerdict(Verdict::tle()));
        } else if (!executionResult.isSuccessful()) {
            return optional<TestCaseVerdict>(TestCaseVerdict(Verdict::rte()));
        }
        return {};
    }
};

}

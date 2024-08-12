#pragma once

#include <istream>
#include <iostream>
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
            string result = StringUtils::split(secondLine, ' ')[0];

            if (!result.empty() && result.back() == '%') {
                optional<double> percentage = StringUtils::toNumber<double>(result.substr(0, result.size() - 1));
                if (!percentage) {
                    throw runtime_error("Unknown percentage format: " + result);
                }
                return {Verdict::ok(), optional<double>(), percentage};
            } else {
                optional<double> points = StringUtils::toNumber<double>(result);
                if (!points) {
                    throw runtime_error("Unknown points format: " + result);
                }
                return {Verdict::ok(), points.value()};
            }
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

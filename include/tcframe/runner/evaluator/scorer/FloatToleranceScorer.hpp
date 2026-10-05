#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "Scorer.hpp"
#include "ScoringResult.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/util.hpp"

using std::istream;
using std::istringstream;
using std::string;
using std::vector;

namespace tcframe {

// Compares expected and contestant output token by token. Tokens that are
// floating-point numbers on both sides may differ within the tolerance; every
// other token must match exactly. Whitespace layout is ignored.
//
// Modes (RFC "Scorer for float tolerance"):
//   ABSOLUTE: |a - b| <= eps
//   RELATIVE: |a - b| <= eps * max(|a|, |b|)
//   BOTH:     either of the above
class FloatToleranceScorer : public Scorer {
public:
    enum class Mode {
        ABSOLUTE,
        RELATIVE,
        BOTH,
    };

private:
    OperatingSystem* os_;
    double absoluteEps_;
    double relativeEps_;
    Mode mode_;

    static bool isFloatToken(const string& token) {
        bool hasDigit = false;
        for (char c : token) {
            if (c >= '0' && c <= '9') {
                hasDigit = true;
            } else if (string("+-.eE").find(c) == string::npos) {
                return false;
            }
        }
        if (!hasDigit) {
            return false;
        }
        char* end = nullptr;
        strtod(token.c_str(), &end);
        return end == token.c_str() + token.size();
    }

    static vector<string> tokenize(const string& content) {
        vector<string> tokens;
        istringstream in(content);
        string token;
        while (in >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool tokensMatch(const string& expected, const string& received) const {
        if (expected == received) {
            return true;
        }
        if (!isFloatToken(expected) || !isFloatToken(received)) {
            return false;
        }

        double a = strtod(expected.c_str(), nullptr);
        double b = strtod(received.c_str(), nullptr);
        double diff = fabs(a - b);
        bool absoluteOk = diff <= absoluteEps_;
        bool relativeOk = diff <= relativeEps_ * std::max(fabs(a), fabs(b));

        switch (mode_) {
            case Mode::ABSOLUTE:
                return absoluteOk;
            case Mode::RELATIVE:
                return relativeOk;
            case Mode::BOTH:
                return absoluteOk || relativeOk;
        }
        return false;
    }

public:
    virtual ~FloatToleranceScorer() = default;

    FloatToleranceScorer(OperatingSystem* os, double eps, Mode mode)
            : FloatToleranceScorer(os, eps, eps, mode) {}

    // absoluteEps is used by ABSOLUTE and BOTH, relativeEps by RELATIVE and BOTH.
    FloatToleranceScorer(OperatingSystem* os, double absoluteEps, double relativeEps, Mode mode)
            : os_(os)
            , absoluteEps_(absoluteEps)
            , relativeEps_(relativeEps)
            , mode_(mode) {}

    ScoringResult score(const string&, const string& outputFilename, const string& evaluationFilename) {
        vector<string> expected = readTokens(outputFilename);
        vector<string> received = readTokens(evaluationFilename);

        string message;
        if (expected.size() != received.size()) {
            message = "Token count: expected " + std::to_string(expected.size())
                    + ", received " + std::to_string(received.size()) + "\n";
        } else {
            for (size_t i = 0; i < expected.size(); i++) {
                if (!tokensMatch(expected[i], received[i])) {
                    message = "Token " + std::to_string(i + 1)
                            + ": expected '" + expected[i]
                            + "', received '" + received[i] + "'\n";
                    break;
                }
            }
        }

        if (message.empty()) {
            return {TestCaseVerdict(Verdict::ac()), ExecutionResult()};
        }
        return {TestCaseVerdict(Verdict::wa()), ExecutionResultBuilder()
                .setStandardError(message)
                .build()};
    }

private:
    vector<string> readTokens(const string& filename) {
        istream* input = os_->openForReading(filename);
        string content = StringUtils::streamToString(input);
        os_->closeOpenedStream(input);
        return tokenize(content);
    }
};

}

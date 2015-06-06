#ifndef TCFRAME_VERDICT_H
#define TCFRAME_VERDICT_H

#include "failure.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;
using tcframe::Failure;

namespace tcframe {

class Verdict {
public:
    Verdict() { }

    static Verdict unknown() {
        return Verdict("Unknown", "?", -1);
    }

    static Verdict accepted() {
        return Verdict("Accepted", "AC", 0);
    }

    static Verdict wrongAnswer(vector<Failure> failures) {
        return Verdict("Wrong Answer", "WA", 1, failures);
    }

    static Verdict runtimeError(vector<Failure> failures) {
        return Verdict("Runtime Error", "RTE", 2, failures);
    }

    static Verdict timeLimitExceeded() {
        return Verdict("Time Limit Exceeded", "TLE", 3);
    }

    bool isUnknown() {
        return code == "?";
    }

    bool isAccepted() {
        return code == "AC";
    }

    string getName() {
        return name;
    }

    string getCode() {
        return code;
    }

    vector<Failure> getFailures() {
        return failures;
    }

    bool operator<(const Verdict& o) const {
        return priority < o.priority;
    }

private:
    Verdict(string name, string code, int priority)
            : name(name), code(code), priority(priority) { }

    Verdict(string name, string code, int priority, vector<Failure> failures)
            : name(name), code(code), priority(priority), failures(failures) { }

    string name;
    string code;
    int priority;
    vector<Failure> failures;
};

}

#endif
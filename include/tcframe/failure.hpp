#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.hpp"
#include "util.hpp"

#include <string>

using std::string;

namespace tcframe {

class Failure {
public:
    Failure(string message, int level)
            : message(message), level(level) { }

    string getMessage() {
        return message;
    }

    int getLevel() {
        return level;
    }

    bool operator==(Failure other) const {
        return message == other.message && level == other.level;
    }

private:
    string message;
    int level;
};

class FailuresCollector {
public:
    void addFailure(Failure failure) {
        failures.push_back(failure);
    }

    void addFailures(vector<Failure> failures) {
        this->failures.insert(this->failures.end(), failures.begin(), failures.end());
    }

    vector<Failure> collectFailures() {
        return failures;
    }

private:
    vector<Failure> failures;
};

}

#endif
#pragma once

#include "tcframe/deprecated/core/Failure.hpp"

#include <exception>
#include <vector>

using std::exception;
using std::vector;

namespace tcframe_old {

class TestCaseException : public exception {
public:
    virtual ~TestCaseException() noexcept { }

    vector<Failure> getFailures() {
        if (message != "") {
            return {Failure(message, 0)};
        } else {
            return failures;
        }
    }

    string getMessage() {
        return message;
    }

protected:
    TestCaseException(string message) {
        this->message = message;
    }

    TestCaseException(vector<Failure> failures) {
        this->failures = failures;
    }

private:
    string message;
    vector<Failure> failures;
};

}

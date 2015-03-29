#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include "failure.hpp"

#include <exception>
#include <string>
#include <vector>

using std::exception;
using std::string;
using std::vector;

namespace tcframe {

class NotImplementedException : public exception { };

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

class IOFormatException : public TestCaseException {
public:
    IOFormatException(string message)
            : TestCaseException(message) { }
};

class ParsingException : public TestCaseException {
public:
    ParsingException(string message)
            : TestCaseException(message) { }
};

class PrintingException : public TestCaseException {
public:
    PrintingException(string message)
            : TestCaseException(message) { }
};

class SubtaskSatisfiabilityException : public TestCaseException {
public:
    SubtaskSatisfiabilityException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

class ExecutionException : public TestCaseException {
public:
    ExecutionException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}

#endif

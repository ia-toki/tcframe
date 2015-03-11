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

class GenerationException : public exception {
public:
    virtual ~GenerationException() { }

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
    GenerationException(string message) {
        this->message = message;
    }

    GenerationException(vector<Failure> failures) {
        this->failures = failures;
    }

private:
    string message;
    vector<Failure> failures;
};

class IOFormatException : public GenerationException {
public:
    IOFormatException(string message)
            : GenerationException(message) { }

    IOFormatException(vector<Failure> failures)
            : GenerationException(failures) { }
};

class TestCaseException : public GenerationException {
public:
    virtual ~TestCaseException() { }

protected:
    TestCaseException(string message)
            : GenerationException(message) { }

    TestCaseException(vector<Failure> failures)
            : GenerationException(failures) { }
};

class ParsingException : public TestCaseException {
public:
    ParsingException(string message)
            : TestCaseException(message) { }

    ParsingException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

class PrintingException : public TestCaseException {
public:
    PrintingException(string message)
            : TestCaseException(message) { }

    PrintingException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

class SubtaskSatisfiabilityException : public TestCaseException {
public:
    SubtaskSatisfiabilityException(string message)
            : TestCaseException(message) { }

    SubtaskSatisfiabilityException(vector<Failure> failures)
            : TestCaseException(failures) { }
};

}

#endif

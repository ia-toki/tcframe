#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include "failure.hpp"
#include "util.hpp"

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
    virtual ~TestCaseException() { }

    TestCaseException(TestCaseFailure* failure)
            : failure(failure) { }

    TestCaseFailure* getFailure() {
        return failure;
    }

private:
    TestCaseFailure* failure;
};

class SubtaskException : public TestCaseException {
public:
    SubtaskException(SubtaskFailure* failure)
            : TestCaseException(failure) { }
};

class IOFormatException : public TestCaseException {
public:
    IOFormatException(IOFormatFailure* failure)
            : TestCaseException(failure) { }

    IOFormatException(string message)
            : TestCaseException(new IOFormatFailure(message)) { }

    string getMessage() {
        return static_cast<IOFormatFailure*>(getFailure())->getMessage();
    }
};

}

#endif

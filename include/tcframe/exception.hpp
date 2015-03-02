#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include "constraint.hpp"
#include "failure.hpp"

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

using std::exception;
using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

class NotImplementedException : public exception { };

class TestCaseException : public exception {
public:
    TestCaseException(vector<Failure*> failures)
            : failures(failures) { }

    vector<Failure*> getFailures() {
        return failures;
    }

private:
    vector<Failure*> failures;
};

class TypeException : public runtime_error {
public:
    TypeException(const string& message) :
        runtime_error(message) { }
};

class IOFormatException : public runtime_error {
public:
    IOFormatException(const string& message)
            : runtime_error(message) { }
};

}

#endif

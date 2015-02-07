#ifndef TCFRAME_EXCEPTION_H
#define TCFRAME_EXCEPTION_H

#include "constraint.h"
#include "failure.h"

#include <exception>
#include <vector>

using std::exception;
using std::vector;

namespace tcframe {

class NotImplementedException : public exception { };

class TestCaseException : public exception {
private:
    vector<Failure*> failures;

public:
    TestCaseException(vector<Failure*> failures) :
        failures(failures) { }

    vector<Failure*> getFailures() {
        return failures;
    }
};

}

#endif

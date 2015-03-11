#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include "failure.hpp"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::string;

namespace tcframe {

class Logger {
public:
    virtual ~Logger() { }

    virtual void logIntroduction() = 0;
    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(string testCaseName) = 0;
    virtual void logTestCaseOkResult() = 0;
    virtual void logTestCaseFailedResult(string testCaseDescription) = 0;
    virtual void logFailures(vector<Failure> failures) = 0;
};

class StandardLogger : public Logger {
public:
    void logIntroduction() override {
        cout << "Generating test cases..." << endl;
    }

    void logTestGroupIntroduction(int testGroupId) override {
        cout << endl;

        if (testGroupId == 0) {
            cout << "[ SAMPLE TEST CASES ]" << endl;
        } else if (testGroupId == -1) {
            cout << "[ OFFICIAL TEST CASES ]" << endl;
        } else {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    void logTestCaseIntroduction(string testCaseName) override {
        cout << "  " << testCaseName << ": " << flush;
    }

    void logTestCaseOkResult() override {
        cout << "OK" << endl;
    }

    void logTestCaseFailedResult(string testCaseDescription) override {
        cout << "FAILED" << endl;

        if (testCaseDescription != "") {
            cout << "    Description: " << testCaseDescription << endl;
        }

        cout << "    Reasons:" << endl;
    }

    void logFailures(vector<Failure> failures) override {
        for (Failure failure : failures) {
            string prefix;
            if (failure.getLevel() == 0) {
                prefix = "    * ";
            } else {
                prefix = "      - ";
            }

            cout << prefix << failure.getMessage() << endl;
        }
    }
};

}

#endif

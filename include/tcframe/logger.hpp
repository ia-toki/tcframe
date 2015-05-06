#ifndef TCFRAME_LOGGER_H
#define TCFRAME_LOGGER_H

#include "failure.hpp"
#include "verdict.hpp"

#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::flush;
using std::map;
using std::string;

namespace tcframe {

class Logger {
public:
    virtual ~Logger() { }

    virtual void logTestGroupIntroduction(int testGroupId) = 0;
    virtual void logTestCaseIntroduction(string testCaseName) = 0;
    virtual void logFailures(vector<Failure> failures) = 0;
};

class GeneratorLogger : public Logger {
public:
    virtual ~GeneratorLogger() { }

    virtual void logIntroduction() = 0;
    virtual void logTestCaseOkResult() = 0;
    virtual void logTestCaseFailedResult(string testCaseDescription) = 0;
};

class SubmitterLogger : public Logger {
public:
    virtual ~SubmitterLogger() { }

    virtual void logIntroduction() = 0;
    virtual void logTestCaseVerdict(Verdict verdict) = 0;
    virtual void logSubmissionResult(map<int, Verdict> subtaskVerdicts) = 0;
    virtual void logPorcelainSubmissionResult(map<int, Verdict> subtaskVerdicts) = 0;
};

class DefaultLoggerUtils {
public:
    static void logTestGroupIntroduction(int testGroupId) {
        cout << endl;

        if (testGroupId == 0) {
            cout << "[ SAMPLE TEST CASES ]" << endl;
        } else if (testGroupId == -1) {
            cout << "[ OFFICIAL TEST CASES ]" << endl;
        } else {
            cout << "[ TEST GROUP " << testGroupId << " ]" << endl;
        }
    }

    static void logTestCaseIntroduction(string testCaseName) {
        cout << "  " << testCaseName << ": " << flush;
    }

    static void logFailures(vector<Failure> failures) {
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

class DefaultGeneratorLogger : public GeneratorLogger {
public:
    void logTestGroupIntroduction(int testGroupId) { DefaultLoggerUtils::logTestGroupIntroduction(testGroupId); }
    void logTestCaseIntroduction(string testCaseName) { DefaultLoggerUtils::logTestCaseIntroduction(testCaseName); }
    void logFailures(vector<Failure> failures) { DefaultLoggerUtils::logFailures(failures); }

    void logIntroduction() {
        cout << "Generating test cases..." << endl;
    }

    void logTestCaseOkResult() {
        cout << "OK" << endl;
    }

    void logTestCaseFailedResult(string testCaseDescription) {
        cout << "FAILED" << endl;

        if (testCaseDescription != "") {
            cout << "    Description: " << testCaseDescription << endl;
        }

        cout << "    Reasons:" << endl;
    }
};

class DefaultSubmitterLogger : public SubmitterLogger {
public:
    void logTestGroupIntroduction(int testGroupId) { DefaultLoggerUtils::logTestGroupIntroduction(testGroupId); }
    void logTestCaseIntroduction(string testCaseName) { DefaultLoggerUtils::logTestCaseIntroduction(testCaseName); }
    void logFailures(vector<Failure> failures) { DefaultLoggerUtils::logFailures(failures); }

    void logIntroduction() {
        cout << "Submitting..." << endl;
    }

    void logTestCaseVerdict(Verdict verdict) {
        cout << verdict.getName() << endl;
    }

    void logSubmissionResult(map<int, Verdict> subtaskVerdicts) {
        cout << endl;
        cout << "[ RESULT ]" << endl;

        for (auto entry : subtaskVerdicts) {
            if (entry.first == -1) {
                cout << entry.second.getName() << endl;
            } else {
                cout << "  Subtask " << entry.first << ": " << entry.second.getName() << endl;
            }
        }
    }

    void logPorcelainSubmissionResult(map<int, Verdict> subtaskVerdicts) {
        for (auto entry : subtaskVerdicts) {
            if (entry.first == -1) {
                cout << entry.second.getCode() << endl;
            } else {
                cout << entry.first << ":" << entry.second.getCode() << endl;
            }
        }
    }
};

}

#endif

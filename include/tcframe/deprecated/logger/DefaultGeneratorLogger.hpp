#pragma once

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/logger/DefaultLoggerUtils.hpp"
#include "tcframe/deprecated/logger/GeneratorLogger.hpp"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

namespace tcframe {

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

    void logMultipleTestCasesCombinationIntroduction(string testCaseBaseName) {
        cout << "  Combining test cases into a single file (" << testCaseBaseName << ")... ";
    }

    void logMultipleTestCasesCombinationOkResult() {
        cout << "OK" << endl;
    }

    void logMultipleTestCasesCombinationFailedResult() {
        cout << "FAILED" << endl;
        cout << "    Reasons:" << endl;
    }
};

}

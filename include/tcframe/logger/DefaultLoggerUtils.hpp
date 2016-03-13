#pragma once

#include "tcframe/core/Failure.hpp"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::flush;
using std::vector;

namespace tcframe {

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

}

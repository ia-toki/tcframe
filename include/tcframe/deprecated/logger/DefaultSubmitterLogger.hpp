#pragma once

#include "tcframe/deprecated/core/Failure.hpp"
#include "tcframe/deprecated/logger/DefaultLoggerUtils.hpp"
#include "tcframe/deprecated/logger/SubmitterLogger.hpp"

#include <iostream>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::vector;

namespace tcframe {

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

    void logBriefSubmissionResult(map<int, Verdict> subtaskVerdicts) {
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

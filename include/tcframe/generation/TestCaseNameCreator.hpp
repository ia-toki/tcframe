#pragma once

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

namespace tcframe {

class TestCaseNameCreator {
public:
    TestCaseNameCreator() = delete;

    static string createSampleTestCaseName(const string &slug, int testCaseId) {
        ostringstream sout;
        sout << slug << "_sample_" << testCaseId;
        return sout.str();
    }

    static string createOfficialTestCaseName(const string &slug, int testGroupId, int testCaseId) {
        ostringstream sout;
        sout << createTestCaseBaseName(slug, testGroupId);
        sout << "_" << testCaseId;
        return sout.str();
    }

private:
    static string createTestCaseBaseName(const string& slug, int testGroupId) {
        ostringstream sout;
        sout << slug;
        if (testGroupId != -1) {
            sout << "_" << testGroupId;
        }
        return sout.str();
    }
};

}

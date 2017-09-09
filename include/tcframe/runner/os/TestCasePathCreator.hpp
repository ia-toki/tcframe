#pragma once

#include <string>

using std::string;

namespace tcframe {

class TestCasePathCreator {
public:
    TestCasePathCreator() = delete;

    static string createInputPath(const string& testCaseName, const string& outputDir) {
        return outputDir + "/" + testCaseName + ".in";
    }

    static string createOutputPath(const string& testCaseName, const string& outputDir) {
        return outputDir + "/" + testCaseName + ".out";
    }
};

}

#pragma once

#include "tcframe/deprecated/generator/TestCase.hpp"

#include <vector>

using std::vector;

namespace tcframe {

class TestGroup {
public:
    TestGroup(int id)
            : id(id) { }

    void addTestCase(TestCase *testCase) {
        testCases.push_back(testCase);
    }

    int getId() {
        return id;
    }

    int getTestCasesCount() {
        return (int) testCases.size();
    }

    TestCase *getTestCase(int index) {
        return testCases[index];
    }

private:
    int id;
    vector<TestCase*> testCases;
};

}

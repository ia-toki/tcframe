#ifndef CPTEST_TESTDATA_H
#define CPTEST_TESTDATA_H

#include <functional>
#include <set>
#include <string>
#include <vector>

using std::function;
using std::set;
using std::string;
using std::vector;

namespace org { namespace iatoki { namespace cptest {

class TestCase {
private:
    function<void()> callback;
    string description;

public:
    TestCase(function<void()> callback, string description) {
        this->callback = callback;
        this->description = description;
    }

    void execute() {
        callback();
    }

    string getDescription() {
        return description;
    }
};


class TestSet {
private:
    vector<TestCase> testCases;
    set<int> subtaskNumbers;

public:
    void addTestCase(TestCase testCase) {
        testCases.push_back(testCase);
    }

    void assignToSubtask(int subtaskNo) {
        subtaskNumbers.insert(subtaskNo);
    }

    vector<TestCase> getTestCases() {
        return testCases;
    }

    set<int> getSubtaskNumbers() {
        return subtaskNumbers;
    }
};

}}}

#endif
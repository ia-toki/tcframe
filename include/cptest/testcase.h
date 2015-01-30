#ifndef CPTEST_TESTCASE_H
#define CPTEST_TESTCASE_H

#include <functional>
#include <set>
#include <string>
#include <vector>
#include <cstdio>

using std::function;
using std::initializer_list;
using std::set;
using std::string;
using std::vector;

namespace org { namespace iatoki { namespace cptest {

struct TestCase {
    int id;
    function<void()> closure;
    string description;
};

struct SampleTestCase {
    int id;
    string content;
    set<int> subtaskNos;
};

struct TestGroup {
    int id;
    vector<TestCase*> testCases;
    set<int> subtaskNos;
};

class TestCasesCollector {
private:
    int curTestGroupNo;
    int curTestCaseNo;
    int curSampleTestCaseNo;

    vector<TestGroup*> testGroups;
    vector<SampleTestCase*> sampleTestCases;

public:
    TestCasesCollector() :
        curTestGroupNo(0),
        curTestCaseNo(0)
    { }

    void newTestGroup() {
        curTestGroupNo++;
        curTestCaseNo = 0;
        testGroups.push_back(new TestGroup{curTestGroupNo});
    }

    void addSampleTestCase(string content) {
        curSampleTestCaseNo++;
        sampleTestCases.push_back(new SampleTestCase{curSampleTestCaseNo, content});
    }


    void addTestCase(function<void()> closure, string description) {
        if (!curTestGroupNo) {
            testGroups.push_back(new TestGroup{0});
        }

        curTestCaseNo++;
        testGroups.back()->testCases.push_back(new TestCase{curTestCaseNo, closure, description});
    }

    void assignToSubtasks(initializer_list<int> subtaskNos) {
        set<int>& curSubtaskNos = testGroups.back()->subtaskNos;

        for (int subtaskNo : subtaskNos) {
            curSubtaskNos.insert(subtaskNo - 1);
        }
    }

    vector<TestGroup*> collectTestData() {
        return testGroups;
    }

    vector<SampleTestCase*> collectSampleTestData() {
        return sampleTestCases;
    }
};

}}}

#endif
#pragma once

#include "tcframe/deprecated/generator/OfficialTestCase.hpp"
#include "tcframe/deprecated/generator/SampleTestCase.hpp"
#include "tcframe/deprecated/generator/TestGroup.hpp"

#include <functional>
#include <initializer_list>
#include <set>
#include <vector>

using std::function;
using std::initializer_list;
using std::set;
using std::vector;

namespace tcframe_old {

class TestCasesCollector {
public:
    TestCasesCollector() {
        testGroups.push_back(new TestGroup(0));
    }

    void newTestGroup() {
        curTestGroupId++;
        curSubtaskIds.clear();
        testGroups.push_back(new TestGroup(curTestGroupId));
    }

    void addSampleTestCase(initializer_list<string> lines, set<int> subtaskIds) {
        string content;
        for (string line : lines) {
            content += line + "\n";
        }

        testGroups.back()->addTestCase(new SampleTestCase(content, subtaskIds));
    }

    void addSampleTestCase(initializer_list<string> lines) {
        addSampleTestCase(lines, {-1});
    }

    void addOfficialTestCase(function<void()> closure, string description) {
        if (testGroups.size() == 1) {
            testGroups.push_back(new TestGroup(-1));
        }

        testGroups.back()->addTestCase(new OfficialTestCase(closure, description, curSubtaskIds));
    }

    void assignToSubtasks(set<int> subtaskIds) {
        this->curSubtaskIds = subtaskIds;
    }

    vector<TestGroup*> collectTestData() {
        return testGroups;
    }

private:
    int curTestGroupId = 0;
    set<int> curSubtaskIds = {-1};

    vector<TestGroup*> testGroups;
};

}

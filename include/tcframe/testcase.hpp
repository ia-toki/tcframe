#ifndef TCFRAME_TESTCASE_H
#define TCFRAME_TESTCASE_H

#include <functional>
#include <set>
#include <string>
#include <vector>

using std::function;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

enum TestCaseType {
    SAMPLE,
    OFFICIAL
};

class TestCase {
public:
    virtual TestCaseType getType() = 0;
    virtual string getDescription() = 0;
    virtual set<int> getSubtaskIds() = 0;

    virtual ~TestCase() { }
};

class OfficialTestCase : public TestCase {
public:
    OfficialTestCase(function<void()> closure, string description, set<int> subtaskIds)
            : closure(closure), description(description), subtaskIds(subtaskIds) { }

    TestCaseType getType() override {
        return TestCaseType::OFFICIAL;
    }

    string getDescription() override {
        return description;
    }

    set<int> getSubtaskIds() override {
        return subtaskIds;
    }

    void apply() {
        closure();
    }

private:
    function<void()> closure;
    string description;
    set<int> subtaskIds;

};

class SampleTestCase : public TestCase {
public:
    SampleTestCase(string content, string description, set<int> subtaskIds) :
        content(content), description(description), subtaskIds(subtaskIds) { }

    TestCaseType getType() override {
        return TestCaseType::SAMPLE;
    }

    string getDescription() override {
        return description;
    }

    set<int> getSubtaskIds() override {
        return subtaskIds;
    }

    string getContent() {
        return content;
    }

private:
    string content;
    string description;
    set<int> subtaskIds;
};

class TestGroup {
public:
    TestGroup(int id)
            : id(id) { }

    void addTestCase(TestCase* testCase) {
        testCases.push_back(testCase);
    }

    int getId() {
        return id;
    }

    int getTestCasesCount() {
        return (int)testCases.size();
    }

    TestCase* getTestCase(int index) {
        return testCases[index];
    }

private:
    int id;
    vector<TestCase*> testCases;
};

class TestCasesCollector {
public:
    TestCasesCollector()
            : curTestGroupId(0), curSubtaskIds({-1}) {
        testGroups.push_back(new TestGroup(0));
    }

    void newTestGroup() {
        curTestGroupId++;
        curSubtaskIds.clear();
        testGroups.push_back(new TestGroup(curTestGroupId));
    }

    void addTestCase(function<void()> closure, string description) {
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
    int curTestGroupId;
    set<int> curSubtaskIds;

    vector<TestGroup*> testGroups;
};

}

#endif

#ifndef TCFRAME_TESTCASE_H
#define TCFRAME_TESTCASE_H

#include <functional>
#include <initializer_list>
#include <set>
#include <string>
#include <vector>

using std::function;
using std::initializer_list;
using std::set;
using std::string;
using std::vector;

namespace tcframe {

class TestCase {
public:
    virtual ~TestCase() { }

    virtual string getDescription()  = 0;
    virtual set<int> getSubtaskIds() = 0;
};

class OfficialTestCase : public TestCase {
public:
    OfficialTestCase(function<void()> closure, string description, set<int> subtaskIds)
            : closure(closure), description(description), subtaskIds(subtaskIds) { }

    string getDescription() {
        return description;
    }

    set<int> getSubtaskIds() {
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
    SampleTestCase(string content, set<int> subtaskIds) :
        content(content), subtaskIds(subtaskIds) { }

    string getDescription() {
        return "";
    }

    set<int> getSubtaskIds() {
        return subtaskIds;
    }

    string getContent() {
        return content;
    }

private:
    string content;
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
    int curTestGroupId;
    set<int> curSubtaskIds;

    vector<TestGroup*> testGroups;
};

}

#endif

#pragma once

#include <functional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "OfficialTestCaseData.hpp"
#include "SampleTestCaseData.hpp"
#include "TestCase.hpp"
#include "TestCaseIdCreator.hpp"
#include "TestGroup.hpp"

using std::function;
using std::set;
using std::string;
using std::tie;
using std::vector;

namespace tcframe {

struct TestSuite {
    friend class TestSuiteBuilder;

private:
    vector<TestGroup> testGroups_;

public:
    TestSuite() {}

    TestSuite(const vector<TestGroup>& testGroups)
            : testGroups_(testGroups) {}

    const vector<TestGroup> testGroups() const {
        return testGroups_;
    }

    bool operator==(const TestSuite& o) const {
        return tie(testGroups_) == tie(o.testGroups_);
    }
};

class TestSuiteBuilder {
private:
    string slug_;
    function<void()> beforeClosure_;
    function<void()> afterClosure_;

    set<int>* curSubtaskIds_;

    int curOfficialTestGroupId_;
    bool hasCurOfficialTestGroup_;
    set<int> curOfficialSubtaskIds_;
    vector<TestGroup> curOfficialTestGroups_;
    vector<TestCase> curOfficialTestCases_;

    bool hasCurSampleTestCase_;
    set<int> curSampleSubtaskIds_;
    vector<TestCase> curSampleTestCases_;
    optional<vector<string>> curSampleInputLines_;
    optional<vector<string>> curSampleOutputLines_;

public:
    virtual ~TestSuiteBuilder() {}

    TestSuiteBuilder()
            : beforeClosure_([]{})
            , afterClosure_([]{})
            , curSampleSubtaskIds_({})
            , curOfficialSubtaskIds_({})
            , curOfficialTestGroupId_(-1)
            , hasCurOfficialTestGroup_(false)
            , hasCurSampleTestCase_(false) {
    }

    TestSuiteBuilder& setSlug(string slug) {
        slug_ = slug;
        return *this;
    }

    TestSuiteBuilder& setBeforeClosure(function<void()> beforeClosure) {
        beforeClosure_ = beforeClosure;
        return *this;
    }

    TestSuiteBuilder& setAfterClosure(function<void()> afterClosure) {
        afterClosure_ = afterClosure;
        return *this;
    }

    TestSuiteBuilder& newSampleTestCase() {
        if (hasCurSampleTestCase_) {
            addCurrentSampleTestCase();
        }

        hasCurSampleTestCase_ = true;
        curSampleSubtaskIds_ = {};
        curSubtaskIds_ = &curSampleSubtaskIds_;
        curSampleInputLines_ = optional<vector<string>>();
        curSampleOutputLines_ = optional<vector<string>>();

        return *this;
    }

    TestSuiteBuilder& newTestGroup() {
        if (hasCurOfficialTestGroup_) {
            addCurrentOfficialTestCase();
        } else {
            curOfficialTestGroupId_ = 0;
        }

        hasCurOfficialTestGroup_ = true;
        curOfficialTestGroupId_++;
        curOfficialSubtaskIds_ = {};
        curSubtaskIds_ = &curOfficialSubtaskIds_;
        curOfficialTestCases_.clear();

        return *this;
    }

    TestSuiteBuilder& Subtasks(set<int> subtaskIds) {
        *curSubtaskIds_ = subtaskIds;
        return *this;
    }

    TestSuiteBuilder& Input(vector<string> lines) {
        curSampleInputLines_ = optional<vector<string>>(lines);
        return *this;
    }

    TestSuiteBuilder& Output(vector<string> lines) {
        curSampleOutputLines_ = optional<vector<string>>(lines);
        return *this;
    }

    TestSuiteBuilder& addOfficialTestCase(function<void()> clozure, string description) {
        // Note: GCC will be angry at you if the parameter name is "closure".
        // https://bugs.archlinux.org/task/35803

        curOfficialTestCases_.push_back(TestCaseBuilder()
                .setId(TestCaseIdCreator::create(slug_, curOfficialTestGroupId_, (int) curOfficialTestCases_.size() + 1))
                .setSubtaskIds(curOfficialSubtaskIds_)
                .setDescription(description)
                .setData(new OfficialTestCaseData([=]{
                    beforeClosure_();
                    clozure();
                    afterClosure_();}))
                .build());

        return *this;
    }

    TestSuite build() {
        if (hasCurSampleTestCase_ && curSampleInputLines_) {
            addCurrentSampleTestCase();
        }
        if (!curOfficialTestCases_.empty()) {
            addCurrentOfficialTestCase();
        }

        vector<TestGroup> testGroups;
        testGroups.push_back(TestGroup(0, curSampleTestCases_));
        for (TestGroup testGroup : curOfficialTestGroups_) {
            testGroups.push_back(testGroup);
        }

        return TestSuite(testGroups);
    }

private:
    void addCurrentSampleTestCase() {
        if (!curSampleInputLines_) {
            // TODO: throw exception
        }

        SampleTestCaseData* data;

        string input;
        for (const string& line : curSampleInputLines_.value()) {
            input += line + "\n";
        }

        if (curSampleOutputLines_) {
            string output;
            for (const string& line : curSampleOutputLines_.value()) {
                output += line + "\n";
            }
            data = new SampleTestCaseData(input, output);
        } else {
            data = new SampleTestCaseData(input);
        }

        curSampleTestCases_.push_back(TestCaseBuilder()
                .setId(TestCaseIdCreator::create(slug_, 0, (int) curSampleTestCases_.size() + 1))
                .setSubtaskIds(curSampleSubtaskIds_)
                .setData(data)
                .build());
    }

    void addCurrentOfficialTestCase() {
        curOfficialTestGroups_.push_back(TestGroup(curOfficialTestGroupId_, curOfficialTestCases_));
    }
};

}

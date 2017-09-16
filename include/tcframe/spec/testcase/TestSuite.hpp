#pragma once

#include <algorithm>
#include <functional>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "OfficialTestCaseData.hpp"
#include "SampleTestCaseData.hpp"
#include "TestCase.hpp"
#include "TestGroup.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/util.hpp"

using std::inserter;
using std::function;
using std::move;
using std::runtime_error;
using std::set;
using std::set_difference;
using std::string;
using std::tie;
using std::vector;

namespace tcframe {

struct TestSuite {
    friend class TestSuiteBuilder;

private:
    vector<TestGroup> testGroups_;

public:
    TestSuite() = default;

    explicit TestSuite(vector<TestGroup> testGroups)
            : testGroups_(move(testGroups)) {}

    const vector<TestGroup>& testGroups() const {
        return testGroups_;
    }

    bool operator==(const TestSuite& o) const {
        return tie(testGroups_) == tie(o.testGroups_);
    }
};

class TestSuiteBuilder {
private:
    string slug_;
    set<int> definedSubtaskIds_;
    function<void()> beforeClosure_ = []{};
    function<void()> afterClosure_ = []{};

    set<int>* curSubtaskIds_ = nullptr;

    int curOfficialTestGroupId_ = TestGroup::MAIN_ID;
    bool hasCurOfficialTestGroup_ = false;
    set<int> curOfficialSubtaskIds_ = {Subtask::MAIN_ID};
    vector<TestGroup> curOfficialTestGroups_;
    vector<TestCase> curOfficialTestCases_;

    bool hasCurSampleTestCase_ = false;
    set<int> curSampleSubtaskIds_ = {Subtask::MAIN_ID};
    vector<TestCase> curSampleTestCases_;
    optional<vector<string>> curSampleInputLines_;
    optional<vector<string>> curSampleOutputLines_;

public:
    TestSuiteBuilder& setSlug(string slug) {
        slug_ = move(slug);
        return *this;
    }

    TestSuiteBuilder& setDefinedSubtaskIds(set<int> definedSubtaskIds) {
        definedSubtaskIds_ = move(definedSubtaskIds);
        return *this;
    }

    TestSuiteBuilder& setBeforeClosure(function<void()> beforeClosure) {
        beforeClosure_ = move(beforeClosure);
        return *this;
    }

    TestSuiteBuilder& setAfterClosure(function<void()> afterClosure) {
        afterClosure_ = move(afterClosure);
        return *this;
    }

    TestSuiteBuilder& newSampleTestCase() {
        if (hasCurSampleTestCase_) {
            addCurrentSampleTestCase();
        }

        hasCurSampleTestCase_ = true;
        curSampleSubtaskIds_ = {};
        curSubtaskIds_ = &curSampleSubtaskIds_;
        curSampleInputLines_ = {};
        curSampleOutputLines_ = {};

        return *this;
    }

    TestSuiteBuilder& newTestGroup() {
        if (hasCurOfficialTestGroup_) {
            addCurrentOfficialTestGroup();
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
        set<int> undefinedSubtaskIds;
        set_difference(
                subtaskIds.begin(),
                subtaskIds.end(),
                definedSubtaskIds_.begin(),
                definedSubtaskIds_.end(),
                inserter(undefinedSubtaskIds, undefinedSubtaskIds.begin()));

        if (!undefinedSubtaskIds.empty()) {
            throw runtime_error(
                    "The following subtasks are referenced but not defined: " +
                    StringUtils::setToString(undefinedSubtaskIds));
        }

        *curSubtaskIds_ = subtaskIds;
        return *this;
    }

    TestSuiteBuilder& Input(vector<string> lines) {
        curSampleInputLines_ = optional<vector<string>>(move(lines));
        return *this;
    }

    TestSuiteBuilder& Output(vector<string> lines) {
        curSampleOutputLines_ = optional<vector<string>>(move(lines));
        return *this;
    }

    TestSuiteBuilder& addOfficialTestCase(function<void()> clozure, string description) {
        // Note: GCC will be angry at you if the parameter name is "closure".
        // https://bugs.archlinux.org/task/35803

        curOfficialTestCases_.push_back(TestCaseBuilder()
                .setName(TestCase::createName(
                        TestGroup::createName(slug_, curOfficialTestGroupId_),
                        (int) curOfficialTestCases_.size() + 1))
                .setSubtaskIds(curOfficialSubtaskIds_)
                .setDescription(move(description))
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
            addCurrentOfficialTestGroup();
        }

        vector<TestGroup> testGroups;
        testGroups.emplace_back(TestGroup::SAMPLE_ID, curSampleTestCases_);
        for (auto testGroup : curOfficialTestGroups_) {
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
                .setName(TestCase::createName(
                        TestGroup::createName(slug_, TestGroup::SAMPLE_ID),
                        (int) curSampleTestCases_.size() + 1))
                .setSubtaskIds(curSampleSubtaskIds_)
                .setData(data)
                .build());
    }

    void addCurrentOfficialTestGroup() {
        curOfficialTestGroups_.emplace_back(curOfficialTestGroupId_, curOfficialTestCases_);
    }
};

}

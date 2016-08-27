#pragma once

#include <functional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "OfficialTestCaseData.hpp"
#include "SampleTestCaseData.hpp"
#include "TestCase.hpp"
#include "TestCaseIdCreator.hpp"
#include "TestGroup.hpp"

using std::function;
using std::move;
using std::set;
using std::string;
using std::tie;

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
    TestSuite subject_;

    string slug_;
    function<void()> inputFinalizer_;
    int curTestGroupId_;
    set<int> curSubtaskIds_;
    vector<TestCase> curTestCases_;

public:
    virtual ~TestSuiteBuilder() {}

    TestSuiteBuilder()
            : inputFinalizer_([]{})
            , curTestGroupId_(0)
            , curSubtaskIds_({-1}) {
    }

    TestSuiteBuilder& setSlug(string slug) {
        slug_ = slug;
        return *this;
    }

    TestSuiteBuilder& setInputFinalizer(function<void()> inputFinalizer) {
        inputFinalizer_ = inputFinalizer;
        return *this;
    }

    TestSuiteBuilder& addSampleTestCase(vector<string> lines, set<int> subtaskIds) {
        string content;
        for (const string& line : lines) {
            content += line + "\n";
        }

        curTestCases_.push_back(TestCaseBuilder()
                .setId(TestCaseIdCreator::create(slug_, 0, (int) curTestCases_.size() + 1))
                .setSubtaskIds(subtaskIds)
                .setData(new SampleTestCaseData(content))
                .build());

        return *this;
    }

    TestSuiteBuilder& addSampleTestCase(vector<string> lines) {
        return addSampleTestCase(lines, {-1});
    }

    TestSuiteBuilder& newTestGroup() {
        subject_.testGroups_.push_back(TestGroup(curTestGroupId_, curTestCases_));

        curTestGroupId_++;
        curSubtaskIds_ = {-1};
        curTestCases_.clear();

        return *this;
    }

    TestSuiteBuilder& Subtasks(set<int> subtaskIds) {
        curSubtaskIds_ = subtaskIds;
        return *this;
    }

    TestSuiteBuilder& addOfficialTestCase(function<void()> clozure, string description) {
        // Note: GCC will be angry at you if the parameter name is "closure".
        // https://bugs.archlinux.org/task/35803

        if (subject_.testGroups_.empty()) {
            subject_.testGroups_.push_back(TestGroup(0, curTestCases_));
            curTestGroupId_ = -1;
            curSubtaskIds_ = {-1};
            curTestCases_.clear();
        }

        curTestCases_.push_back(TestCaseBuilder()
                .setId(TestCaseIdCreator::create(slug_, curTestGroupId_, (int) curTestCases_.size() + 1))
                .setSubtaskIds(curSubtaskIds_)
                .setDescription(description)
                .setData(new OfficialTestCaseData([=]{clozure(); inputFinalizer_();}))
                .build());

        return *this;
    }

    TestSuite build() {
        subject_.testGroups_.push_back(TestGroup(curTestGroupId_, curTestCases_));
        return move(subject_);
    }

    TestSuite buildWithoutLastTestGroup() {
        return move(subject_);
    }
};

}

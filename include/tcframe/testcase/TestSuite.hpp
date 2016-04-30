#pragma once

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "OfficialTestCase.hpp"
#include "SampleTestCase.hpp"
#include "TestGroup.hpp"

using std::move;
using std::set;
using std::string;

namespace tcframe {

struct TestSuite {
    friend class TestSuiteBuilder;

private:
    vector<SampleTestCase> sampleTests_;
    vector<TestGroup> officialTests_;

public:
    const vector<SampleTestCase> sampleTests() const {
        return sampleTests_;
    }

    const vector<TestGroup>& officialTests() const {
        return officialTests_;
    }
};

class TestSuiteBuilder {
private:
    TestSuite subject_;

    bool hasCurrentTestGroup_;
    int currentTestGroupId_;
    set<int> currentConstraintGroupIds_;
    vector<OfficialTestCase> currentOfficialTestCases_;

public:
    virtual ~TestSuiteBuilder() {}

    TestSuiteBuilder()
            : hasCurrentTestGroup_(false)
            , currentTestGroupId_(0)
            , currentConstraintGroupIds_({-1})
    {}

    TestSuiteBuilder& addSampleTestCase(const vector<string>& lines, const set<int>& constraintGroupIds) {
        string content;
        for (const string& line : lines) {
            content += line + "\n";
        }
        subject_.sampleTests_.push_back(SampleTestCase(content, constraintGroupIds));

        return *this;
    }

    TestSuiteBuilder& addSampleTestCase(const vector<string>& lines) {
        return addSampleTestCase(lines, {-1});
    }

    TestSuiteBuilder& newTestGroup() {
        if (hasCurrentTestGroup_) {
            subject_.officialTests_.push_back(TestGroup(currentTestGroupId_, currentConstraintGroupIds_, currentOfficialTestCases_));
        }

        hasCurrentTestGroup_ = true;
        currentTestGroupId_++;
        currentConstraintGroupIds_ = {-1};
        currentOfficialTestCases_.clear();

        return *this;
    }

    TestSuiteBuilder& setConstraintGroupIds(const set<int>& constraintGroupIds) {
        currentConstraintGroupIds_ = constraintGroupIds;

        return *this;
    }

    TestSuiteBuilder& addOfficialTestCase(OfficialTestCase officialTestCase) {
        if (!hasCurrentTestGroup_) {
            hasCurrentTestGroup_ = true;
            currentTestGroupId_ = -1;
            currentConstraintGroupIds_ = {-1};
        }

        currentOfficialTestCases_.push_back(officialTestCase);

        return *this;
    }

    TestSuite build() {
        if (hasCurrentTestGroup_) {
            subject_.officialTests_.push_back(TestGroup(currentTestGroupId_, currentConstraintGroupIds_, currentOfficialTestCases_));
        }
        return move(subject_);
    }

    TestSuite buildWithoutLastTestGroup() {
        return move(subject_);
    }
};

}

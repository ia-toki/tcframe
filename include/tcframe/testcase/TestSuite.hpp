#pragma once

#include <initializer_list>
#include <set>
#include <string>
#include <utility>

#include "TestCase.hpp"
#include "TestGroup.hpp"

using std::initializer_list;
using std::move;
using std::set;
using std::string;

namespace tcframe {

struct TestSuite {
    friend class TestSuiteBuilder;

private:
    vector<TestGroup> testCases_;

public:
    const vector<TestGroup>& testCases() const {
        return testCases_;
    }
};

class TestSuiteBuilder {
private:
    TestSuite subject_;

    bool hasCurrentTestGroup_;
    int currentTestGroupId_;
    set<int> currentConstraintGroupIds_;
    vector<TestCase> currentTestCases_;

public:
    TestSuiteBuilder()
            : hasCurrentTestGroup_(false)
            , currentTestGroupId_(0)
            , currentConstraintGroupIds_({-1})
    {}

    TestSuiteBuilder& newTestGroup() {
        if (hasCurrentTestGroup_) {
            subject_.testCases_.push_back(TestGroup(currentTestGroupId_, currentConstraintGroupIds_, currentTestCases_));
        }

        hasCurrentTestGroup_ = true;
        currentTestGroupId_++;
        currentConstraintGroupIds_ = {-1};
        currentTestCases_.clear();

        return *this;
    }

    TestSuiteBuilder& setConstraintGroupIds(initializer_list<int> constraintGroupIds) {
        currentConstraintGroupIds_ = constraintGroupIds;

        return *this;
    }

    TestSuiteBuilder& addTestCase(TestCase testCase) {
        if (!hasCurrentTestGroup_) {
            hasCurrentTestGroup_ = true;
            currentTestGroupId_ = -1;
            currentConstraintGroupIds_ = {-1};
        }

        currentTestCases_.push_back(testCase);

        return *this;
    }

    TestSuite build() {
        if (hasCurrentTestGroup_) {
            subject_.testCases_.push_back(TestGroup(currentTestGroupId_, currentConstraintGroupIds_, currentTestCases_));
        }
        return subject_;
    }
};

}

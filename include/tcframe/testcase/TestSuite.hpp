#pragma once

#include <initializer_list>
#include <set>
#include <string>
#include <utility>

#include "OfficialTestCase.hpp"
#include "TestGroup.hpp"

using std::initializer_list;
using std::move;
using std::set;
using std::string;

namespace tcframe {

struct TestSuite {
    friend class TestSuiteBuilder;

private:
    vector<TestGroup> officialTests_;

public:
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
    TestSuiteBuilder()
            : hasCurrentTestGroup_(false)
            , currentTestGroupId_(0)
            , currentConstraintGroupIds_({-1})
    {}

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

    TestSuiteBuilder& setConstraintGroupIds(initializer_list<int> constraintGroupIds) {
        currentConstraintGroupIds_ = constraintGroupIds;

        return *this;
    }

    TestSuiteBuilder& addOfficialTestCase(OfficialTestCase testCase) {
        if (!hasCurrentTestGroup_) {
            hasCurrentTestGroup_ = true;
            currentTestGroupId_ = -1;
            currentConstraintGroupIds_ = {-1};
        }

        currentOfficialTestCases_.push_back(testCase);

        return *this;
    }

    TestSuite build() {
        if (hasCurrentTestGroup_) {
            subject_.officialTests_.push_back(TestGroup(currentTestGroupId_, currentConstraintGroupIds_, currentOfficialTestCases_));
        }
        return subject_;
    }
};

}

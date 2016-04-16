#pragma once

#include <set>
#include <utility>
#include <vector>

#include "tcframe/testcase/TestCase.hpp"

using std::move;
using std::set;
using std::vector;

namespace tcframe {

struct TestGroup {
private:
    int id_;
    set<int> constraintGroupIds_;
    vector<TestCase> testCases_;

public:
    TestGroup(int id, set<int> constraintGroupId, vector<TestCase> testCases)
            : id_(id)
            , constraintGroupIds_(move(constraintGroupId))
            , testCases_(move(testCases))
    {}

    int id() const {
        return id_;
    }

    const set<int>& constraintGroupIds() const {
        return constraintGroupIds_;
    }

    const vector<TestCase>& testCases() const {
        return testCases_;
    }
};

}

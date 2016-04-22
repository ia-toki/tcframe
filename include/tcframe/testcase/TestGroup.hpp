#pragma once

#include <set>
#include <tuple>
#include <vector>

#include "TestCase.hpp"

using std::set;
using std::tie;
using std::vector;

namespace tcframe {

struct TestGroup {
private:
    int id_;
    set<int> constraintGroupIds_;
    vector<TestCase> testCases_;

public:
    TestGroup(int id, const set<int>& constraintGroupId, const vector<TestCase>& testCases)
            : id_(id)
            , constraintGroupIds_(constraintGroupId)
            , testCases_(testCases)
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

    bool operator==(const TestGroup& o) const {
        return tie(id_, constraintGroupIds_, testCases_) == tie(o.id_, o.constraintGroupIds_, o.testCases_);
    }
};

}

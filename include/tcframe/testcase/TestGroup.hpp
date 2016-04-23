#pragma once

#include <set>
#include <tuple>
#include <vector>

#include "OfficialTestCase.hpp"

using std::set;
using std::tie;
using std::vector;

namespace tcframe {

struct TestGroup {
private:
    int id_;
    set<int> constraintGroupIds_;
    vector<OfficialTestCase> officialTestCases_;

public:
    TestGroup(int id, const set<int>& constraintGroupId, const vector<OfficialTestCase>& testCases)
            : id_(id)
            , constraintGroupIds_(constraintGroupId)
            , officialTestCases_(testCases)
    {}

    int id() const {
        return id_;
    }

    const set<int>& constraintGroupIds() const {
        return constraintGroupIds_;
    }

    const vector<OfficialTestCase>& officialTestCases() const {
        return officialTestCases_;
    }

    bool operator==(const TestGroup& o) const {
        return tie(id_, constraintGroupIds_, officialTestCases_) == tie(o.id_, o.constraintGroupIds_, o.officialTestCases_);
    }
};

}

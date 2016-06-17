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
    set<int> subtaskIds_;
    vector<OfficialTestCase> officialTestCases_;

public:
    TestGroup(int id, const set<int>& subtaskIds, const vector<OfficialTestCase>& testCases)
            : id_(id)
            , subtaskIds_(subtaskIds)
            , officialTestCases_(testCases)
    {}

    int id() const {
        return id_;
    }

    const set<int>& subtaskIds() const {
        return subtaskIds_;
    }

    const vector<OfficialTestCase>& officialTestCases() const {
        return officialTestCases_;
    }

    bool operator==(const TestGroup& o) const {
        return tie(id_, subtaskIds_, officialTestCases_) == tie(o.id_, o.subtaskIds_, o.officialTestCases_);
    }
};

}

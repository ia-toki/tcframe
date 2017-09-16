#pragma once

#include <ostream>
#include <tuple>
#include <utility>
#include <vector>

#include "TestCase.hpp"

using std::move;
using std::tie;
using std::vector;

namespace tcframe {

struct TestGroup {
    enum {
        MAIN_ID = -1,
        SAMPLE_ID = 0
    };

private:
    int id_;
    vector<TestCase> testCases_;

public:
    TestGroup(int id, vector<TestCase> testCases)
            : id_(id)
            , testCases_(move(testCases)) {}

    int id() const {
        return id_;
    }

    const vector<TestCase>& testCases() const {
        return testCases_;
    }

    bool operator==(const TestGroup& o) const {
        return tie(id_, testCases_) == tie(o.id_, o.testCases_);
    }

    static string createName(const string& slug, int testGroupId) {
        if (testGroupId == SAMPLE_ID) {
            return slug + "_sample";
        }
        if (testGroupId != MAIN_ID) {
            return slug + "_" + StringUtils::toString(testGroupId);
        }
        return slug;
    }
};

}

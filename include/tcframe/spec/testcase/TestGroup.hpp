#pragma once

#include <ostream>
#include <tuple>
#include <vector>

#include "TestCase.hpp"

using std::tie;
using std::vector;

namespace tcframe {

struct TestGroup {
private:
    int id_;
    vector<TestCase> testCases_;

public:
    TestGroup(int id, const vector<TestCase>& testCases)
            : id_(id)
            , testCases_(testCases) {}

    int id() const {
        return id_;
    }

    const vector<TestCase>& testCases() const {
        return testCases_;
    }

    bool operator==(const TestGroup& o) const {
        return tie(id_, testCases_) == tie(o.id_, o.testCases_);
    }
};

}

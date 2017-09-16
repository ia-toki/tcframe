#pragma once

#include <functional>
#include <utility>

#include "TestCaseData.hpp"

using std::function;
using std::move;

namespace tcframe {

struct OfficialTestCaseData : public TestCaseData {
private:
    function<void()> closure_;

public:
    virtual ~OfficialTestCaseData() = default;

    explicit OfficialTestCaseData(function<void()> closure)
            : closure_(move(closure)) {}

    TestCaseDataType type() const {
        return TestCaseDataType::OFFICIAL;
    }

    const function<void()>& closure() const {
        return closure_;
    }

    bool operator==(const OfficialTestCaseData& o) const {
        return true;
    }

    bool equals(TestCaseData* o) const {
        return o->type() == TestCaseDataType::OFFICIAL && *this == *((OfficialTestCaseData*) o);
    }
};

}

#pragma once

#include <string>
#include <tuple>

#include "TestCaseData.hpp"

using std::string;
using std::tie;

namespace tcframe {

struct SampleTestCaseData : public TestCaseData {
private:
    string content_;

public:
    SampleTestCaseData(const string& content)
              : content_(content) {}

    TestCaseDataType type() const {
        return TestCaseDataType::SAMPLE;
    }

    const string& content() const {
        return content_;
    }

    bool operator==(const SampleTestCaseData& o) const {
        return tie(content_) == tie(o.content_);
    }

    bool equals(TestCaseData* o) const {
        return o->type() == TestCaseDataType::SAMPLE && *this == *((SampleTestCaseData*) o);
    }
};

}

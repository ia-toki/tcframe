#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "TestCaseData.hpp"
#include "tcframe/util.hpp"

using std::string;
using std::tie;

namespace tcframe {

struct SampleTestCaseData : public TestCaseData {
private:
    string input_;
    optional<string> output_;

public:
    virtual ~SampleTestCaseData() = default;

    explicit SampleTestCaseData(string input)
            : input_(move(input)) {}

    SampleTestCaseData(string input, string output)
              : input_(move(input))
              , output_(optional<string>(move(output))) {}

    TestCaseDataType type() const {
        return TestCaseDataType::SAMPLE;
    }

    const string& input() const {
        return input_;
    }

    const optional<string>& output() const {
        return output_;
    }

    bool operator==(const SampleTestCaseData& o) const {
        return tie(input_, output_) == tie(o.input_, o.output_);
    }

    bool equals(TestCaseData* o) const {
        return o->type() == TestCaseDataType::SAMPLE && *this == *((SampleTestCaseData*) o);
    }
};

}

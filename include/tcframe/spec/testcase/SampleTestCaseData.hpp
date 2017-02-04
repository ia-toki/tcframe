#pragma once

#include <functional>
#include <string>
#include <tuple>

#include "TestCaseData.hpp"
#include "tcframe/util.hpp"

using std::function;
using std::string;
using std::tie;

namespace tcframe {

struct SampleTestCaseData : public TestCaseData {
private:
    string input_;
    optional<string> output_;
    function<void()> beforeClosure_;
    function<void()> afterClosure_;

public:
    SampleTestCaseData(
            const function<void()>& beforeClosure,
            const function<void()>& afterClosure,
            const string& input)
            : beforeClosure_(beforeClosure)
            , afterClosure_(afterClosure)
            , input_(input) {}

    SampleTestCaseData(
            const function<void()>& beforeClosure,
            const function<void()>& afterClosure,
            const string& input,
            const string& output)
            : beforeClosure_(beforeClosure)
            , afterClosure_(afterClosure)
            , input_(input)
            , output_(optional<string>(output)) {}

    TestCaseDataType type() const {
        return TestCaseDataType::SAMPLE;
    }

    const function<void()>& beforeClosure() const {
        return beforeClosure_;
    }

    const function<void()>& afterClosure() const {
        return afterClosure_;
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

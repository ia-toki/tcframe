#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct MultipleTestCasesConfig {
    friend class MultipleTestCasesConfigBuilder;

private:
    optional<int*> counter_;
    optional<string> outputPrefix_;

public:
    const optional<int*>& counter() const {
        return counter_;
    }

    const optional<string>& outputPrefix() const {
        return outputPrefix_;
    }

    bool operator==(const MultipleTestCasesConfig& o) const {
        return tie(counter_, outputPrefix_) == tie(o.counter_, o.outputPrefix_);
    }
};

class MultipleTestCasesConfigBuilder {
private:
    MultipleTestCasesConfig subject_;

public:
    MultipleTestCasesConfigBuilder& Counter(int& var) {
        subject_.counter_ = optional<int*>(&var);
        return *this;
    }

    MultipleTestCasesConfigBuilder& OutputPrefix(string outputPrefix) {
        subject_.outputPrefix_ = optional<string>(move(outputPrefix));
        return *this;
    }

    MultipleTestCasesConfig build() {
        return move(subject_);
    }
};

}

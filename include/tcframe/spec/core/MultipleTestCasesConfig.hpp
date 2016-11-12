#pragma once

#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct MultipleTestCasesConfig {
    friend class MultipleTestCasesConfigBuilder;

private:
    optional<int*> counter_;

public:
    const optional<int*>& counter() const {
        return counter_;
    }

    bool operator==(const MultipleTestCasesConfig& o) const {
        return tie(counter_) == tie(o.counter_);
    }
};

class MultipleTestCasesConfigBuilder {
private:
    MultipleTestCasesConfig subject_;

public:
    virtual ~MultipleTestCasesConfigBuilder() {}

    MultipleTestCasesConfigBuilder& Counter(int& var) {
        subject_.counter_ = optional<int*>(&var);
        return *this;
    }

    MultipleTestCasesConfig build() {
        return move(subject_);
    }
};

}

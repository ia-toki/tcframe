#pragma once

#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct ProblemConfig {
    friend class ProblemConfigBuilder;

private:
    optional<int*> multipleTestCasesCount_;

public:
    const optional<int*>& multipleTestCasesCount() const {
        return multipleTestCasesCount_;
    }

    bool operator==(const ProblemConfig& o) const {
        return tie(multipleTestCasesCount_) == tie(o.multipleTestCasesCount_);
    }
};

class ProblemConfigBuilder {
private:
    ProblemConfig subject_;

public:
    virtual ~ProblemConfigBuilder() {}

    ProblemConfigBuilder& MultipleTestCasesCount(int& var) {
        subject_.multipleTestCasesCount_ = optional<int*>(&var);
        return *this;
    }

    ProblemConfig build() {
        return move(subject_);
    }
};

}

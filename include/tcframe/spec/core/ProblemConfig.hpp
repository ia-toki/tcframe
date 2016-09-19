#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ProblemConfig {
    friend class ProblemConfigBuilder;

private:
    optional<int*> multipleTestCasesCount_;
    optional<string> slug_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const optional<int*>& multipleTestCasesCount() const {
        return multipleTestCasesCount_;
    }

    const optional<string>& slug() const {
        return slug_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const ProblemConfig& o) const {
        return tie(multipleTestCasesCount_, slug_) == tie(o.multipleTestCasesCount_, o.slug_);
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

    ProblemConfigBuilder& Slug(string slug) {
        subject_.slug_ = optional<string>(slug);
        return *this;
    }

    ProblemConfigBuilder& TimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    ProblemConfigBuilder& MemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    ProblemConfig build() {
        return move(subject_);
    }
};

}

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

public:
    const optional<int*>& multipleTestCasesCount() const {
        return multipleTestCasesCount_;
    }

    const optional<string>& slug() const {
        return slug_;
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

    ProblemConfigBuilder& setMultipleTestCasesCount(int& var) {
        subject_.multipleTestCasesCount_ = optional<int*>(&var);
        return *this;
    }

    ProblemConfigBuilder& setSlug(string slug) {
        subject_.slug_ = optional<string>(slug);
        return *this;
    }

    ProblemConfig build() {
        return move(subject_);
    }
};

}

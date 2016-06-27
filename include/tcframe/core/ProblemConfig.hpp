#pragma once

#include <string>
#include <tuple>
#include <utility>

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ProblemConfig {
    friend class ProblemConfigBuilder;

private:
    string slug_;
    int* multipleTestCasesCount_;

public:
    const string& slug() const {
        return slug_;
    }

    int* multipleTestCasesCount() const {
        return multipleTestCasesCount_;
    }

    bool operator==(const ProblemConfig& o) const {
        return tie(slug_) == tie(o.slug_);
    }
};

class ProblemConfigBuilder {
private:
    ProblemConfig subject_;

public:
    virtual ~ProblemConfigBuilder() {}

    ProblemConfigBuilder() {
        subject_.slug_ = "problem";
        subject_.multipleTestCasesCount_ = nullptr;
    }

    ProblemConfigBuilder& setSlug(string slug) {
        subject_.slug_ = slug;
        return *this;
    }

    ProblemConfigBuilder& setMultipleTestCasesCount(int& var) {
        subject_.multipleTestCasesCount_ = &var;
        return *this;
    }

    ProblemConfig build() {
        return move(subject_);
    }
};

}

#pragma once

#include <string>
#include <utility>

using std::move;
using std::string;

namespace tcframe { namespace experimental {

struct ProblemConfig {
    friend class ProblemConfigBuilder;

private:
    string slug_;

public:
    const string& slug() const {
        return slug_;
    }
};

class ProblemConfigBuilder {
private:
    ProblemConfig subject_;

public:
    ProblemConfigBuilder() {
        subject_.slug_ = "problem";
    }

    void setSlug(string slug) {
        subject_.slug_ = slug;
    }

    ProblemConfig build() {
        return move(subject_);
    }
};

}}

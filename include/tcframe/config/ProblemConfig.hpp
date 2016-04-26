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

public:
    const string& slug() const {
        return slug_;
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
    }

    ProblemConfigBuilder& setSlug(string slug) {
        subject_.slug_ = slug;
        return *this;
    }

    ProblemConfig build() {
        return subject_;
    }
};

}

#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/verdict.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct ScoringResult {
    friend class ScoringResultBuilder;

private:
    Verdict verdict_;
    string message_;

public:
    const Verdict& verdict() const {
        return verdict_;
    }

    const string& message() const {
        return message_;
    }

    bool operator==(const ScoringResult& o) const {
        return tie(verdict_, message_) == tie(o.verdict_, o.message_);
    }
};

class ScoringResultBuilder {
private:
    ScoringResult subject_;

public:
    ScoringResultBuilder& setVerdict(Verdict verdict) {
        subject_.verdict_ = verdict;
        return *this;
    }

    ScoringResultBuilder& setMessage(string message) {
        subject_.message_ = message;
        return *this;
    }

    ScoringResult build() {
        return move(subject_);
    }
};

}

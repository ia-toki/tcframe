#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct Metadata {
    friend class MetadataBuilder;

private:
    string slug_;
    optional<int> timeLimit_;
    optional<int> memoryLimit_;

public:
    const string& slug() const {
        return slug_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool operator==(const Metadata& o) const {
        return tie(slug_, timeLimit_, memoryLimit_) == tie(o.slug_, o.timeLimit_, o.memoryLimit_);
    }
};

class MetadataBuilder {
private:
    Metadata subject_;

public:
    MetadataBuilder(const string& slug) {
        subject_.slug_ = slug;
    }

public:
    MetadataBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    MetadataBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    Metadata build() {
        return move(subject_);
    }
};

}

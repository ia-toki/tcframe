#pragma once

#include <string>
#include <tuple>
#include <utility>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct Config {
    friend class ConfigBuilder;

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

    bool operator==(const Config& o) const {
        return tie(slug_, timeLimit_, memoryLimit_) == tie(o.slug_, o.timeLimit_, o.memoryLimit_);
    }
};

class ConfigBuilder {
private:
    Config subject_;

public:
    ConfigBuilder(const string& slug) {
        subject_.slug_ = slug;
    }

public:
    ConfigBuilder& setTimeLimit(int timeLimit) {
        subject_.timeLimit_ = optional<int>(timeLimit);
        return *this;
    }

    ConfigBuilder& setMemoryLimit(int memoryLimit) {
        subject_.memoryLimit_ = optional<int>(memoryLimit);
        return *this;
    }

    Config build() {
        return move(subject_);
    }
};

}

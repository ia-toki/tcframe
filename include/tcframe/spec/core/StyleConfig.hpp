#pragma once

#include <tuple>
#include <utility>

#include "CommonConfig.hpp"

using std::move;
using std::tie;

namespace tcframe {

struct StyleConfig {
    friend class StyleConfigBuilder;

private:
    bool needsOutput_;

public:
    bool needsOutput() const {
        return needsOutput_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(needsOutput_) == tie(o.needsOutput_);
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    virtual ~StyleConfigBuilder() {}

    StyleConfigBuilder() {
        subject_.needsOutput_ = CommonConfig::needsOutput();
    }

    StyleConfigBuilder& NoOutput() {
        subject_.needsOutput_ = false;
        return *this;
    }

    StyleConfig build() {
        return move(subject_);
    }
};

}

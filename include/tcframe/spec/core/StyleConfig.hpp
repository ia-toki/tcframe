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
    bool needsCustomScorer_;
    bool needsOutput_;

public:
    bool needsCustomScorer() const {
        return needsCustomScorer_;
    }

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
        subject_.needsCustomScorer_ = CommonConfig::needsCustomScorer();
    }

    StyleConfigBuilder& CustomScorer() {
        subject_.needsCustomScorer_ = true;
        return *this;
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

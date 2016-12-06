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
    bool needsOutputFiles_;

public:
    bool needsOutputFiles() const {
        return needsOutputFiles_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(needsOutputFiles_) == tie(o.needsOutputFiles_);
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    virtual ~StyleConfigBuilder() {}

    StyleConfigBuilder() {
        subject_.needsOutputFiles_ = CommonConfig::generateOutput();
    }

    StyleConfigBuilder& NoOutput() {
        subject_.needsOutputFiles_ = false;
        return *this;
    }

    StyleConfig build() {
        return move(subject_);
    }
};

}

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
    bool generateOutputFiles_;

public:
    bool needsOutputFiles() const {
        return generateOutputFiles_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(generateOutputFiles_) == tie(o.generateOutputFiles_);
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    virtual ~StyleConfigBuilder() {}

    StyleConfigBuilder() {
        subject_.generateOutputFiles_ = CommonConfig::generateOutput();
    }

    StyleConfigBuilder& NoOutputFiles() {
        subject_.generateOutputFiles_ = false;
        return *this;
    }

    StyleConfig build() {
        return move(subject_);
    }
};

}

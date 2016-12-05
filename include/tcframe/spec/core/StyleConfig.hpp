#pragma once

#include <tuple>
#include <utility>

using std::move;
using std::tie;

namespace tcframe {

struct StyleConfig {
    friend class StyleConfigBuilder;

private:
    bool generateOutputFiles_;

public:
    bool generateOutputFiles() const {
        return generateOutputFiles_;
    }

    bool operator==(const StyleConfig& o) const {
        return generateOutputFiles_ == o.generateOutputFiles_;
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    virtual ~StyleConfigBuilder() {}

    StyleConfigBuilder() {
        subject_.generateOutputFiles_ = true;
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

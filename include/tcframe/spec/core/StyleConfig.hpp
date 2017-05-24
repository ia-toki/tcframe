#pragma once

#include <tuple>
#include <utility>

#include "CommonConfig.hpp"

using std::move;
using std::tie;

namespace tcframe {

enum class EvaluationStyle {
    BATCH,
    INTERACTIVE
};

struct StyleConfig {
    friend class StyleConfigBuilder;

private:
    EvaluationStyle evaluationStyle_;
    bool needsCustomScorer_;
    bool needsOutput_;

public:
    EvaluationStyle evaluationStyle() const {
        return evaluationStyle_;
    }

    bool needsCustomScorer() const {
        return needsCustomScorer_;
    }

    bool needsOutput() const {
        return needsOutput_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(evaluationStyle_, needsCustomScorer_, needsOutput_)
               == tie(o.evaluationStyle_, o.needsCustomScorer_, o.needsOutput_);
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    virtual ~StyleConfigBuilder() {}

    StyleConfigBuilder() {
        subject_.evaluationStyle_ = EvaluationStyle::BATCH;
        subject_.needsOutput_ = true;
        subject_.needsCustomScorer_ = false;
    }

    StyleConfigBuilder& BatchEvaluator() {
        subject_.evaluationStyle_ = EvaluationStyle::BATCH;
        return *this;
    }

    StyleConfigBuilder& InteractiveEvaluator() {
        subject_.evaluationStyle_ = EvaluationStyle::INTERACTIVE;
        return *this;
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

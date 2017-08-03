#pragma once

#include <tuple>
#include <utility>

using std::move;
using std::tie;

namespace tcframe {

enum class EvaluationStyle {
    BATCH,
    INTERACTIVE
};

struct StyleConfig {
    friend class StyleConfigBuilder;

public:
    static const EvaluationStyle DEFAULT_EVALUATION_STYLE = EvaluationStyle::BATCH;
    static const bool DEFAULT_NEEDS_OUTPUT = true;
    static const bool DEFAULT_NEEDS_CUSTOM_SCORER = false;

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
    StyleConfigBuilder() {
        subject_.evaluationStyle_ = StyleConfig::DEFAULT_EVALUATION_STYLE;
        subject_.needsOutput_ = StyleConfig::DEFAULT_NEEDS_OUTPUT;
        subject_.needsCustomScorer_ = StyleConfig::DEFAULT_NEEDS_CUSTOM_SCORER;
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

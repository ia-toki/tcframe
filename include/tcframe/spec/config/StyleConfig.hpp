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
    static const bool DEFAULT_HAS_TC_OUTPUT = true;
    static const bool DEFAULT_HAS_SCORER = false;

private:
    EvaluationStyle evaluationStyle_;
    bool hasScorer_;
    bool hasTcOutput_;

public:
    EvaluationStyle evaluationStyle() const {
        return evaluationStyle_;
    }

    bool hasScorer() const {
        return hasScorer_;
    }

    bool hasTcOutput() const {
        return hasTcOutput_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(evaluationStyle_, hasScorer_, hasTcOutput_)
               == tie(o.evaluationStyle_, o.hasScorer_, o.hasTcOutput_);
    }
};

class StyleConfigBuilder {
private:
    StyleConfig subject_;

public:
    StyleConfigBuilder() {
        subject_.evaluationStyle_ = StyleConfig::DEFAULT_EVALUATION_STYLE;
        subject_.hasTcOutput_ = StyleConfig::DEFAULT_HAS_TC_OUTPUT;
        subject_.hasScorer_ = StyleConfig::DEFAULT_HAS_SCORER;
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
        subject_.hasScorer_ = true;
        return *this;
    }

    StyleConfigBuilder& NoOutput() {
        subject_.hasTcOutput_ = false;
        return *this;
    }

    StyleConfig build() {
        return move(subject_);
    }
};

}

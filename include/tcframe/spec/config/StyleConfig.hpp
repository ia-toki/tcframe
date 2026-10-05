#pragma once

#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "tcframe/util.hpp"

using std::move;
using std::string;
using std::tie;
using std::vector;

namespace tcframe {

enum class EvaluationStyle {
    BATCH,
    INTERACTIVE,
    OUTPUT_ONLY,
    FUNCTIONAL
};

enum class FloatToleranceMode {
    ABSOLUTE,
    RELATIVE,
    BOTH
};

// Float tolerance of the default scorer: tolerance = 10^-k (RFC "Scorer for float tolerance").
struct FloatTolerance {
    int k;
    FloatToleranceMode mode;

    bool operator==(const FloatTolerance& o) const {
        return tie(k, mode) == tie(o.k, o.mode);
    }
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
    optional<FloatTolerance> floatTolerance_;
    vector<string> solutionKeys_;

public:
    EvaluationStyle evaluationStyle() const {
        return evaluationStyle_;
    }

    const vector<string>& solutionKeys() const {
        return solutionKeys_;
    }

    bool hasScorer() const {
        return hasScorer_;
    }

    bool hasTcOutput() const {
        return hasTcOutput_;
    }

    const optional<FloatTolerance>& floatTolerance() const {
        return floatTolerance_;
    }

    bool operator==(const StyleConfig& o) const {
        return tie(evaluationStyle_, hasScorer_, hasTcOutput_, floatTolerance_, solutionKeys_)
               == tie(o.evaluationStyle_, o.hasScorer_, o.hasTcOutput_, o.floatTolerance_, o.solutionKeys_);
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
        subject_.solutionKeys_ = {"source"};
    }

    StyleConfigBuilder& BatchEvaluator() {
        subject_.evaluationStyle_ = EvaluationStyle::BATCH;
        return *this;
    }

    StyleConfigBuilder& InteractiveEvaluator() {
        subject_.evaluationStyle_ = EvaluationStyle::INTERACTIVE;
        return *this;
    }

    StyleConfigBuilder& OutputOnlyEvaluator() {
        subject_.evaluationStyle_ = EvaluationStyle::OUTPUT_ONLY;
        return *this;
    }

    StyleConfigBuilder& FunctionalEvaluator(vector<string> solutionKeys) {
        subject_.evaluationStyle_ = EvaluationStyle::FUNCTIONAL;
        subject_.solutionKeys_ = move(solutionKeys);
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

    StyleConfigBuilder& AbsoluteFloatToleranceScorer(int k) {
        subject_.floatTolerance_ = optional<FloatTolerance>(FloatTolerance{k, FloatToleranceMode::ABSOLUTE});
        return *this;
    }

    StyleConfigBuilder& RelativeFloatToleranceScorer(int k) {
        subject_.floatTolerance_ = optional<FloatTolerance>(FloatTolerance{k, FloatToleranceMode::RELATIVE});
        return *this;
    }

    StyleConfigBuilder& FloatToleranceScorer(int k) {
        subject_.floatTolerance_ = optional<FloatTolerance>(FloatTolerance{k, FloatToleranceMode::BOTH});
        return *this;
    }

    StyleConfig build() {
        return move(subject_);
    }
};

}

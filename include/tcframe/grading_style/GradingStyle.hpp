#pragma once

#include <string>

#include "tcframe/evaluator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/scorer.hpp"
#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

struct GradingStyle {
private:
    Evaluator* evaluator_;
    Scorer* scorer_;

public:
    GradingStyle(Evaluator* evaluator, Scorer* scorer)
            : evaluator_(evaluator)
            , scorer_(scorer) {}

    Evaluator* evaluator() const {
        return evaluator_;
    }

    Scorer* scorer() const {
        return scorer_;
    }
};

class GradingStyleFactory {
public:
    virtual ~GradingStyleFactory() {}

    virtual GradingStyle createBatch(OperatingSystem* os, const optional<string>& scorerCommand) {
        Evaluator* evaluator = new BatchEvaluator(os);

        Scorer* scorer;
        if (scorerCommand) {
            scorer = new CustomScorer(os, scorerCommand.value());
        } else {
            scorer = new DiffScorer(os);
        }

        return GradingStyle(evaluator, scorer);
    }
};

}

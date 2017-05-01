#pragma once

#include <string>

#include "BatchEvaluator.hpp"
#include "Evaluator.hpp"
#include "scorer.hpp"
#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::string;

namespace tcframe {

class EvaluatorFactory {
public:
    virtual ~EvaluatorFactory() {}

    virtual Evaluator* createBatch(OperatingSystem* os, const optional<string>& scorerCommand) {
        VerdictCreator* verdictCreator = new VerdictCreator();
        Scorer* scorer;
        if (scorerCommand) {
            scorer = new CustomScorer(os, verdictCreator, scorerCommand.value());
        } else {
            scorer = new DiffScorer(os);
        }

        return new BatchEvaluator(os, verdictCreator, scorer);
    }
};

}

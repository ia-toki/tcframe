#pragma once

#include <string>

#include "tcframe/os.hpp"
#include "tcframe/util.hpp"
#include "scorer.hpp"

using std::string;

namespace tcframe {

class EvaluatorHelperRegistry {
public:
    virtual ~EvaluatorHelperRegistry() {}

    virtual Scorer* getScorer(OperatingSystem* os, const optional<string>& scorerCommand) {
        if (scorerCommand) {
            return new CustomScorer(os, new VerdictCreator(), scorerCommand.value());
        } else {
            return new DiffScorer(os);
        }
    }
};

}

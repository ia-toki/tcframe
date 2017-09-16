#pragma once

#include <string>

#include "tcframe/runner/os.hpp"
#include "tcframe/util.hpp"
#include "communicator.hpp"
#include "scorer.hpp"

using std::string;

namespace tcframe {

class EvaluatorHelperRegistry {
public:
    virtual ~EvaluatorHelperRegistry() = default;

    virtual Scorer* getScorer(OperatingSystem* os, const optional<string>& scorerCommand) {
        if (scorerCommand) {
            return new CustomScorer(os, new VerdictCreator(), scorerCommand.value());
        } else {
            return new DiffScorer(os);
        }
    }

    virtual Communicator* getCommunicator(OperatingSystem* os, const string& communicatorCommand) {
        return new Communicator(os, new VerdictCreator(), communicatorCommand);
    }
};

}

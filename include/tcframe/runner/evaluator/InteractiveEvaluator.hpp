#pragma once

#include <string>

#include "EvaluationOptions.hpp"
#include "EvaluationResult.hpp"
#include "Evaluator.hpp"
#include "communicator.hpp"

using std::string;

namespace tcframe {

class InteractiveEvaluator : public Evaluator {
private:
    Communicator* communicator_;

public:
    virtual ~InteractiveEvaluator() {}

    InteractiveEvaluator(Communicator* communicator)
            : communicator_(communicator) {}

    EvaluationResult evaluate(const string& inputFilename, const string&, const EvaluationOptions& options) {
        CommunicationResult communicationResult = communicator_->communicate(inputFilename, options);
        return EvaluationResult(
                communicationResult.verdict(),
                {{"communicator", communicationResult.executionResult()}});
    }
};

}

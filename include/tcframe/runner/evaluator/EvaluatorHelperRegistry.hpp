#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include "tcframe/runner/os.hpp"
#include "tcframe/util.hpp"
#include "communicator.hpp"
#include "scorer.hpp"

using std::istringstream;
using std::runtime_error;
using std::string;

namespace tcframe {

class EvaluatorHelperRegistry {
public:
    virtual ~EvaluatorHelperRegistry() = default;

    // additionalArgs: the scorer helper's args from spec.yml. A custom scorer program wins.
    // Without one, float tolerance options select FloatToleranceScorer; otherwise the diff scorer.
    virtual Scorer* getScorer(OperatingSystem* os, const optional<string>& scorerCommand, const string& additionalArgs = "") {
        if (scorerCommand) {
            return new CustomScorer(os, new TestCaseVerdictParser(), scorerCommand.value());
        }

        bool hasAbsolute = false;
        bool hasRelative = false;
        double absoluteEps = 0;
        double relativeEps = 0;
        istringstream in(additionalArgs);
        string option;
        while (in >> option) {
            if (option == "float_absolute_tolerance") {
                absoluteEps = readFloatArg(in, option);
                hasAbsolute = true;
            } else if (option == "float_relative_tolerance") {
                relativeEps = readFloatArg(in, option);
                hasRelative = true;
            }
        }

        if (hasAbsolute && hasRelative) {
            return new FloatToleranceScorer(os, absoluteEps, relativeEps, FloatToleranceScorer::Mode::BOTH);
        }
        if (hasAbsolute) {
            return new FloatToleranceScorer(os, absoluteEps, absoluteEps, FloatToleranceScorer::Mode::ABSOLUTE);
        }
        if (hasRelative) {
            return new FloatToleranceScorer(os, relativeEps, relativeEps, FloatToleranceScorer::Mode::RELATIVE);
        }
        return new DiffScorer(os);
    }

    virtual Communicator* getCommunicator(OperatingSystem* os, const string& communicatorCommand) {
        return new Communicator(os, new TestCaseVerdictParser(), communicatorCommand);
    }

private:
    static double readFloatArg(istringstream& in, const string& option) {
        double value;
        if (!(in >> value)) {
            throw runtime_error("tcframe: " + option + " expects a number in the scorer args");
        }
        return value;
    }
};

}

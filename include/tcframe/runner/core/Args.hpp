#pragma once

#include <string>
#include <vector>

#include "tcframe/util.hpp"

using std::string;
using std::vector;

namespace tcframe {

struct Args {
    friend class ArgsParser;

public:
    enum class Command {
        GENERATE,
        GRADE,
        SPEC,
        VALIDATE
    };

private:
    Command command_;

    bool brief_ = false;
    optional<string> communicator_;
    optional<int> memoryLimit_;
    bool noMemoryLimit_ = false;
    bool noTimeLimit_ = false;
    optional<string> scorer_;
    optional<unsigned> seed_;
    optional<string> solution_;
    vector<string> solutionFiles_;
    optional<string> manager_;
    optional<string> evaluatorDir_;
    optional<string> solutionFamily_;
    optional<int> timeLimit_;
    optional<string> output_;
    optional<string> format_;
    optional<string> specFile_;

    // spec-command overrides supplied by the Python CLI (SPEC.md T0.4)
    optional<string> solutionKeys_;
    vector<string> helpers_;
    vector<string> helperArgs_;
    vector<string> aggregators_;
    vector<string> aggregatorArgs_;

public:
    Command command() const {
        return command_;
    }

    bool brief() const {
        return brief_;
    }

    const optional<string>& communicator() const {
        return communicator_;
    }

    const optional<int>& memoryLimit() const {
        return memoryLimit_;
    }

    bool noMemoryLimit() const {
        return noMemoryLimit_;
    }

    bool noTimeLimit() const {
        return noTimeLimit_;
    }

    const optional<string>& output() const {
        return output_;
    }

    const optional<string>& format() const {
        return format_;
    }

    const optional<string>& specFile() const {
        return specFile_;
    }

    const optional<string>& solutionKeys() const {
        return solutionKeys_;
    }

    const vector<string>& helpers() const {
        return helpers_;
    }

    const vector<string>& helperArgs() const {
        return helperArgs_;
    }

    const vector<string>& aggregators() const {
        return aggregators_;
    }

    const vector<string>& aggregatorArgs() const {
        return aggregatorArgs_;
    }

    const optional<string>& scorer() const {
        return scorer_;
    }

    const optional<unsigned>& seed() const {
        return seed_;
    }

    const optional<string>& solution() const {
        return solution_;
    }

    const vector<string>& solutionFiles() const {
        return solutionFiles_;
    }

    const optional<string>& manager() const {
        return manager_;
    }

    const optional<string>& evaluatorDir() const {
        return evaluatorDir_;
    }

    const optional<string>& solutionFamily() const {
        return solutionFamily_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }
};

}

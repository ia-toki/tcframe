#pragma once

#include <string>

#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

struct Args {
    friend class ArgsParser;

public:
    enum class Command {
        GENERATE,
        GRADE
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
    optional<int> timeLimit_;
    optional<string> output_;
    bool strict_ = false;

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

    const optional<string>& scorer() const {
        return scorer_;
    }

    const optional<unsigned>& seed() const {
        return seed_;
    }

    const optional<string>& solution() const {
        return solution_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }

    bool strict() const {
        return strict_;
    }

    void useStrict() {
        strict_ = true;
    }

    bool operator==(const Args& o) const {
        return command_ == o.command_
            && brief_ == o.brief_
            && communicator_ == o.communicator_
            && memoryLimit_ == o.memoryLimit_
            && noMemoryLimit_ == o.noMemoryLimit_
            && noTimeLimit_ == o.noTimeLimit_
            && scorer_ == o.scorer_
            && seed_ == o.seed_
            && solution_ == o.solution_
            && timeLimit_ == o.timeLimit_
            && output_ == o.output_
            && strict_ == o.strict_;
    }
};

}

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

    bool brief_;
    optional<string> communicator_;
    optional<int> memoryLimit_;
    bool noMemoryLimit_;
    bool noTimeLimit_;
    optional<string> scorer_;
    optional<unsigned> seed_;
    optional<string> solution_;
    optional<int> timeLimit_;
    optional<string> output_;

public:
    Args()
            : brief_(false)
            , noMemoryLimit_(false)
            , noTimeLimit_(false) {}

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
};

}

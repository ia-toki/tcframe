#pragma once

#include <string>

#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

struct Args {
    friend class ArgsParser;

public:
    enum class Command {
        GEN,
        SUBMIT
    };

private:
    Command command_;

    optional<int> memoryLimit_;
    bool noMemoryLimit_;
    bool noTimeLimit_;
    optional<unsigned> seed_;
    optional<string> solution_;
    optional<string> output_;
    optional<int> timeLimit_;

public:
    Args()
            : noMemoryLimit_(false)
            , noTimeLimit_(false) {}

    Command command() const {
        return command_;
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

    const optional<unsigned>& seed() const {
        return seed_;
    }

    const optional<string>& solution() const {
        return solution_;
    }

    const optional<string>& output() const {
        return output_;
    }

    const optional<int>& timeLimit() const {
        return timeLimit_;
    }
};

}

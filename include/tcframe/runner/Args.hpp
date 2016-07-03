#pragma once

#include <string>

#include "tcframe/util.hpp"

using std::string;

namespace tcframe {

struct Args {
    friend class ArgsParser;

public:
    enum Command {
        GEN
    };

private:
    Command command_;
    optional<string> slug_;
    optional<string> solution_;
    optional<string> tcDir_;
    optional<unsigned> seed_;

public:
    Command command() const {
        return command_;
    }

    const optional<string>& slug() const {
        return slug_;
    }

    const optional<string>& solution() const {
        return solution_;
    }

    const optional<string>& tcDir() const {
        return tcDir_;
    }

    const optional<unsigned>& seed() const {
        return seed_;
    }
};

}

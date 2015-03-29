#ifndef TCFRAME_FAILURE_H
#define TCFRAME_FAILURE_H

#include "constraint.hpp"
#include "util.hpp"

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe {

class Failure {
public:
    Failure(string message, int level)
            : message(message), level(level) { }

    string getMessage() {
        return message;
    }

    int getLevel() {
        return level;
    }

    bool operator==(Failure other) const {
        return message == other.message && level == other.level;
    }

private:
    string message;
    int level;
};

}

#endif
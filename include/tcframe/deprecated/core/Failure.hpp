#pragma once

#include <string>

using std::string;

namespace tcframe_old {

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

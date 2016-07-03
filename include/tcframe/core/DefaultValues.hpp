#pragma once

#include <string>

using std::string;

namespace tcframe {

class DefaultValues {
public:
    static unsigned seed() {
        return 0;
    }

    static string slug() {
        return "problem";
    }

    static string solutionCommand() {
        return "./solution";
    }

    static string testCasesDir() {
        return "tc";
    }
};

}

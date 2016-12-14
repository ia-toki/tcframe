#pragma once

#include <string>

using std::string;

namespace tcframe {

class CommonConfig {
public:
    static unsigned seed() {
        return 0;
    }

    static string solutionCommand() {
        return "./solution";
    }

    static string scorerCommand() {
        return "./scorer";
    }

    static string outputDir() {
        return "tc";
    }

    static bool needsOutput() {
        return true;
    }
};

}

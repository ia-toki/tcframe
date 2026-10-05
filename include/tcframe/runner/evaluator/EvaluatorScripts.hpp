#pragma once

#include <string>
#include <vector>

#include "tcframe/runner/os.hpp"

using std::string;
using std::vector;

namespace tcframe {

// Custom build/run scripts of an evaluator directory (RFC 2.0, SPEC.md T6.3).
//
// - build_[lang]: called with the solution files, then the helper args. It must leave the
//   artifact at the fixed path ./__tcframe_functional (that name is the contract with run_[lang]).
// - run_[lang]: called with the test case input, then the helper args. It runs the artifact.
//
// Without a script, the evaluator falls back to its built-in behaviour for the family.
class EvaluatorScripts {
private:
    OperatingSystem* os_;
    string dir_;

public:
    EvaluatorScripts(OperatingSystem* os, const string& dir)
            : os_(os)
            , dir_(dir) {}

    bool hasBuild(const string& family) {
        return hasScript("build_" + family);
    }

    bool hasRun(const string& family) {
        return hasScript("run_" + family);
    }

    string buildCommand(const string& family, const vector<string>& solutionFiles, const vector<string>& helperArgs) const {
        return command("build_" + family, solutionFiles, helperArgs);
    }

    string runCommand(const string& family, const string& inputFilename, const vector<string>& helperArgs) const {
        return command("run_" + family, {inputFilename}, helperArgs);
    }

private:
    string command(const string& script, const vector<string>& firstArgs, const vector<string>& helperArgs) const {
        string result = dir_ + "/" + script;
        for (const string& arg : firstArgs) {
            result += " " + arg;
        }
        for (const string& arg : helperArgs) {
            result += " " + arg;
        }
        return result;
    }

    bool hasScript(const string& script) {
        if (dir_.empty()) {
            return false;
        }
        // OperatingSystem::openForReading always returns a stream; a missing file leaves it not good.
        istream* in = os_->openForReading(dir_ + "/" + script);
        bool exists = in != nullptr && in->good();
        if (in != nullptr) {
            os_->closeOpenedStream(in);
        }
        return exists;
    }
};

}

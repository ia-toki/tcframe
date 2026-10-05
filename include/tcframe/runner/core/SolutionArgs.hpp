#pragma once

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

#include "Args.hpp"
#include "tcframe/runner/evaluator/SolutionMap.hpp"

using std::runtime_error;
using std::sort;
using std::string;
using std::vector;

namespace tcframe {

// Builds the solution map of a grade/generate run from the CLI (SPEC.md T6.1). Formats:
//   --solution=<command>              (key "source")
//   --solution-file=<key>=<path>      (repeatable; one entry per solution key)
// With neither, the default command is the "source" solution, as in 1.x.
class SolutionArgs {
public:
    SolutionArgs() = delete;

    static SolutionMap build(const Args& args, const string& defaultCommand, const vector<string>& requiredKeys) {
        SolutionMap solutions;
        if (args.solutionFiles().empty()) {
            solutions = SolutionMap(args.solution().value_or(defaultCommand));
        } else {
            if (args.solution()) {
                solutions.set(SolutionMap::DEFAULT_KEY, args.solution().value());
            }
            for (const string& entry : args.solutionFiles()) {
                string key;
                string filename;
                parseEntry(entry, key, filename);
                if (solutions.has(key)) {
                    throw runtime_error("tcframe: duplicate solution for key '" + key + "'");
                }
                solutions.set(key, filename);
            }
        }

        vector<string> expected = requiredKeys;
        sort(expected.begin(), expected.end());
        vector<string> actual = solutions.keys();
        if (expected != actual) {
            throw runtime_error(
                    "tcframe: solution keys mismatch: spec requires [" + join(expected) + "], got [" + join(actual) +
                    "]; pass --solution-file=<key>=<path> for each required key");
        }

        return solutions;
    }

private:
    static void parseEntry(const string& entry, string& key, string& filename) {
        size_t eq = entry.find('=');
        if (eq == string::npos) {
            throw runtime_error("tcframe: --solution-file must be <key>=<path>, got '" + entry + "'");
        }
        key = entry.substr(0, eq);
        filename = entry.substr(eq + 1);

        if (key.empty() || !std::all_of(key.begin(), key.end(), isKeyChar)) {
            throw runtime_error("tcframe: --solution-file has an invalid key in '" + entry + "'");
        }
        if (filename.empty()) {
            throw runtime_error("tcframe: --solution-file has an empty path in '" + entry + "'");
        }
    }

    static bool isKeyChar(char c) {
        return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
    }

    static string join(const vector<string>& items) {
        string result;
        for (size_t i = 0; i < items.size(); i++) {
            if (i > 0) {
                result += ", ";
            }
            result += items[i];
        }
        return result;
    }
};

}

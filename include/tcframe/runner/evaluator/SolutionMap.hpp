#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using std::map;
using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

// A solution is a map of (key, filename) (RFC 2.0, SPEC.md T6.1). By default the only key is "source",
// which is what a 1.x solution command maps to.
class SolutionMap {
public:
    static constexpr const char* DEFAULT_KEY = "source";

private:
    map<string, string> files_;

public:
    SolutionMap() = default;

    explicit SolutionMap(const string& command) {
        files_[DEFAULT_KEY] = command;
    }

    void set(const string& key, const string& filename) {
        files_[key] = filename;
    }

    bool has(const string& key) const {
        return files_.count(key) > 0;
    }

    const string& get(const string& key) const {
        auto it = files_.find(key);
        if (it == files_.end()) {
            throw runtime_error("tcframe: no solution for key '" + key + "'");
        }
        return it->second;
    }

    // The 1.x solution command: the "source" file, or empty when unset (as before SolutionMap existed).
    string defaultCommand() const {
        return has(DEFAULT_KEY) ? files_.at(DEFAULT_KEY) : "";
    }

    // Sorted, since the underlying map is ordered.
    vector<string> keys() const {
        vector<string> keys;
        for (const auto& entry : files_) {
            keys.push_back(entry.first);
        }
        return keys;
    }

    bool operator==(const SolutionMap& o) const {
        return files_ == o.files_;
    }
};

}

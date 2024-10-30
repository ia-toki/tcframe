#pragma once

#include <string>
#include <tuple>
#include <utility>

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct Verdict {
private:
    string code_;
    string name_;
    int priority_;

    Verdict(string code, string name, int priority)
            : code_(move(code))
            , name_(move(name))
            , priority_(priority) {}
public:
    Verdict() = default;

    static Verdict ac() {
        return {"AC", "Accepted", 0};
    }

    static Verdict ok() {
        return {"OK", "OK", 1};
    }

    static Verdict wa() {
        return {"WA", "Wrong Answer", 2};
    }

    static Verdict rte() {
        return {"RTE", "Runtime Error", 3};
    }

    static Verdict tle() {
        return {"TLE", "Time Limit Exceeded", 4};
    }

    static Verdict err() {
        return {"ERR", "Internal Error", 99};
    }

    static Verdict skip() {
        return {"SKIP", "Skipped", -1};
    }

    const string& code() const {
        return code_;
    }

    const string& name() const {
        return name_;
    }

    bool operator==(const Verdict& o) const {
        return tie(code_) == tie(o.code_);
    }

    bool operator<(const Verdict& o) const {
        return priority_ < o.priority_;
    }
};

}

#pragma once

#include <string>
#include <tuple>
#include <utility>

using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct VerdictStatus {
private:
    string code_;
    string name_;
    int priority_;

    VerdictStatus(string code, string name, int priority)
            : code_(move(code))
            , name_(move(name))
            , priority_(priority) {}
public:
    VerdictStatus() = default;

    static VerdictStatus ac() {
        return {"AC", "Accepted", 0};
    }

    static VerdictStatus ok() {
        return {"OK", "OK", 1};
    }

    static VerdictStatus wa() {
        return {"WA", "Wrong Answer", 2};
    }

    static VerdictStatus rte() {
        return {"RTE", "Runtime Error", 3};
    }

    static VerdictStatus tle() {
        return {"TLE", "Time Limit Exceeded", 4};
    }

    static VerdictStatus err() {
        return {"ERR", "Internal Error", 99};
    }

    const string& code() const {
        return code_;
    }

    const string& name() const {
        return name_;
    }

    bool operator==(const VerdictStatus& o) const {
        return tie(code_) == tie(o.code_);
    }

    bool operator<(const VerdictStatus& o) const {
        return priority_ < o.priority_;
    }
};

}

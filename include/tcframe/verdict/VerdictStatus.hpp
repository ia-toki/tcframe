#pragma once

#include <string>
#include <tuple>

using std::string;
using std::tie;

namespace tcframe {

struct VerdictStatus {
private:
    string code_;
    string name_;
    int priority_;

    VerdictStatus(const string& code, const string& name, int priority)
            : code_(code)
            , name_(name)
            , priority_(priority) {}
public:
    VerdictStatus() {}

    static VerdictStatus ac() {
        return VerdictStatus("AC", "Accepted", 0);
    }

    static VerdictStatus ok() {
        return VerdictStatus("OK", "OK", 1);
    }

    static VerdictStatus wa() {
        return VerdictStatus("WA", "Wrong Answer", 2);
    }

    static VerdictStatus rte() {
        return VerdictStatus("RTE", "Runtime Error", 3);
    }

    static VerdictStatus tle() {
        return VerdictStatus("TLE", "Time Limit Exceeded", 4);
    }

    static VerdictStatus err() {
        return VerdictStatus("ERR", "Internal Error", 99);
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

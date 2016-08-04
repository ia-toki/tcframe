#pragma once

#include <string>
#include <tuple>

using std::string;
using std::tie;

namespace tcframe {

struct Verdict {
private:
    string code_;
    string name_;
    int priority_;

public:
    Verdict() {}

    Verdict(const string& code, const string& name, int priority)
            : code_(code)
            , name_(name)
            , priority_(priority) {}

    static Verdict ac() {
        return Verdict("AC", "Accepted", 0);
    }

    static Verdict wa() {
        return Verdict("WA", "Wrong Answer", 1);
    }

    static Verdict rte() {
        return Verdict("RTE", "Runtime Error", 2);
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

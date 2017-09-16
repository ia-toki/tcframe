#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <utility>

using std::function;
using std::move;
using std::string;
using std::tie;

namespace tcframe {

struct Constraint {
private:
    function<bool()> predicate_;
    string description_;

public:
    Constraint(function<bool()> predicate, string description)
            : predicate_(move(predicate))
            , description_(move(description)) {}

    const function<bool()>& predicate() const {
        return predicate_;
    }

    const string& description() const {
        return description_;
    }

    bool operator==(const Constraint& o) const {
        return tie(description_) == tie(o.description_);
    }
};

}

#pragma once

#include <functional>
#include <string>
#include <utility>

using std::function;
using std::move;
using std::string;

namespace tcframe { namespace experimental {

struct Constraint {
private:
    function<bool()> predicate_;
    string description_;

public:
    Constraint(function<bool()> predicate, string description)
            : predicate_(move(predicate))
            , description_(move(description))
    {}

    const function<bool()>& predicate() const {
        return predicate_;
    }

    const string& description() const {
        return description_;
    }
};

}}

#pragma once

#include <functional>
#include <string>

using std::function;
using std::string;

namespace tcframe { namespace experimental {

struct Constraint {
private:
    function<bool()> predicate_;
    string description_;

public:
    Constraint(const function<bool()>& predicate, const string& description)
            : predicate_(predicate)
            , description_(description)
    {}

    const function<bool()>& predicate() const {
        return predicate_;
    }

    const string& description() const {
        return description_;
    }
};

}}

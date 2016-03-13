#pragma once

#include <functional>
#include <string>

using std::function;
using std::string;

namespace tcframe {

class Constraint {
public:
    Constraint(function<bool()> predicate, string description)
            : predicate(predicate), description(description) { }

    bool isSatisfied() {
        return predicate();
    }

    string getDescription() {
        return description;
    }

private:
    function<bool()> predicate;
    string description;
};

}

#pragma once

#include "tcframe/deprecated/generator/TestCase.hpp"

#include <functional>
#include <set>
#include <string>

using std::function;
using std::set;
using std::string;

namespace tcframe_old {

class OfficialTestCase : public TestCase {
public:
    OfficialTestCase(function<void()> closure, string description, set<int> subtaskIds)
            : closure(closure), description(description), subtaskIds(subtaskIds) { }

    string getDescription() {
        return description;
    }

    set<int> getSubtaskIds() {
        return subtaskIds;
    }

    void apply() {
        closure();
    }

private:
    function<void()> closure;
    string description;
    set<int> subtaskIds;
};

}

#pragma once

#include "tcframe/generator/TestCase.hpp"

#include <set>
#include <string>

using std::set;
using std::string;

namespace tcframe {

class SampleTestCase : public TestCase {
public:
    SampleTestCase(string content, set<int> subtaskIds) :
            content(content), subtaskIds(subtaskIds) { }

    string getDescription() {
        return "";
    }

    set<int> getSubtaskIds() {
        return subtaskIds;
    }

    string getContent() {
        return content;
    }

private:
    string content;
    set<int> subtaskIds;
};

}

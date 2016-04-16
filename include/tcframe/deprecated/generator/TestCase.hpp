#pragma once

#include <set>
#include <string>

using std::set;
using std::string;

namespace tcframe {

class TestCase {
public:
    virtual ~TestCase() { }

    virtual string getDescription()  = 0;
    virtual set<int> getSubtaskIds() = 0;
};

}

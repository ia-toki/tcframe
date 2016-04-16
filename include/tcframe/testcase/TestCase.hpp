#pragma once

#include <functional>
#include <string>
#include <tuple>

using std::function;
using std::string;
using std::tie;

namespace tcframe {

struct TestCase {
private:
    function<void()> closure_;
    string description_;

public:
    TestCase(const function<void()>& closure, const string& description)
            : closure_(closure)
            , description_(description)
    {}

    const function<void()>& closure() const {
        return closure_;
    }

    const string& description() const {
        return description_;
    }

    bool operator==(const TestCase& o) const {
        return tie(description_) == tie(o.description_);
    }
};

}

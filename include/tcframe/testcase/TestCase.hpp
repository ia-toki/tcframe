#pragma once

#include <functional>
#include <string>
#include <utility>

using std::function;
using std::move;
using std::string;

namespace tcframe {

struct TestCase {
private:
    function<void()> closure_;
    string description_;

public:
    TestCase(function<void()> closure, string description)
            : closure_(move(closure))
            , description_(move(description))
    {}

    const function<void()>& closure() const {
        return closure_;
    }

    const string& description() const {
        return description_;
    }
};

}

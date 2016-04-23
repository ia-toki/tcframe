#pragma once

#include <functional>
#include <string>
#include <tuple>

using std::function;
using std::string;
using std::tie;

namespace tcframe {

struct OfficialTestCase {
private:
    function<void()> closure_;
    string description_;

public:
    OfficialTestCase(const function<void()>& closure, const string& description)
            : closure_(closure)
            , description_(description)
    {}

    const function<void()>& closure() const {
        return closure_;
    }

    const string& description() const {
        return description_;
    }

    bool operator==(const OfficialTestCase& o) const {
        return tie(description_) == tie(o.description_);
    }
};

}

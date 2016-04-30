#pragma once

#include <set>
#include <string>
#include <tuple>

using std::set;
using std::string;
using std::tie;

namespace tcframe {

struct SampleTestCase {
private:
    string content_;
    set<int> constraintGroupIds_;

public:
    SampleTestCase(const string& content, const set<int>& constraintGroupIds)
              : content_(content)
              , constraintGroupIds_(constraintGroupIds)
    {}

    const string& content() const {
        return content_;
    }

    const set<int>& constraintGroupIds() const {
        return constraintGroupIds_;
    }

    bool operator==(const SampleTestCase& o) const {
        return tie(content_, constraintGroupIds_) == tie(o.content_, constraintGroupIds_);
    }
};

}

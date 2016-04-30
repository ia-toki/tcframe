#pragma once

#include <algorithm>
#include <queue>
#include <string>

#include "tcframe/format.hpp"
#include "tcframe/type.hpp"
#include "tcframe/util.hpp"

using std::forward;
using std::queue;
using std::string;

#define CONS(predicate) addConstraint(Constraint([=] {return predicate;}, #predicate))
#define CASE(...) addOfficialTestCase(OfficialTestCase([=] {__VA_ARGS__;}, #__VA_ARGS__))
#define SAMPLE_CASE(...) addSampleTestCase(__VA_ARGS__)
#define LINE(...) addIOSegment((MagicLineIOSegmentBuilder(#__VA_ARGS__), __VA_ARGS__).build())

namespace tcframe {

class MagicLineIOSegmentBuilder {
private:
    LineIOSegmentBuilder builder_;
    queue<string> names_;

public:
    MagicLineIOSegmentBuilder(string names) {
        for (string name : StringUtils::split(names)) {
            names_.push(name);
        }
    }

    template<typename T>
    MagicLineIOSegmentBuilder& operator,(T&& var) {
        string name = names_.front();
        names_.pop();
        builder_.addVariable(forward<T>(var), name);

        return *this;
    }

    LineIOSegment* build() {
        return builder_.build();
    }
};

}

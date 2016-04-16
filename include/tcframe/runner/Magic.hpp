#pragma once

#include <algorithm>
#include <queue>
#include <string>

#include "tcframe/io/LineIOSegment.hpp"
#include "tcframe/type/Scalar.hpp"
#include "tcframe/util/StringUtils.hpp"

using std::forward;
using std::queue;
using std::string;

#define CONS(predicate) addConstraint(Constraint([=] {return predicate;}, #predicate))
#define CASE(...) addTestCase(TestCase([=] {__VA_ARGS__;}, #__VA_ARGS__))
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

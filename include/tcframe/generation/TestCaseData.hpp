#pragma once

#include <functional>
#include <set>
#include <string>
#include <utility>

using std::function;
using std::move;
using std::set;
using std::string;

namespace tcframe { namespace experimental {

struct TestCaseData {
    friend class TestCaseDataBuilder;

private:
    string name_;
    string description_;
    set<int> constraintGroupIds_;

public:
    const string& name() const {
        return name_;
    }

    const string& description() const {
        return description_;
    }

    const set<int>& constraintGroupIds() const {
        return constraintGroupIds_;
    }
};

class TestCaseDataBuilder {
private:
    TestCaseData subject_;

public:
    TestCaseDataBuilder& setName(string name) {
        subject_.name_ = name;
        return *this;
    }

    TestCaseDataBuilder& setDescription(string description) {
        subject_.description_ = description;
        return *this;
    }

    TestCaseDataBuilder& setConstraintGroupIds(set<int> constraintGroupsId) {
        subject_.constraintGroupIds_ = constraintGroupsId;
        return *this;
    }

    TestCaseData build() {
        return move(subject_);
    }
};

}}

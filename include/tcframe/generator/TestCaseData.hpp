#pragma once

#include <set>
#include <string>
#include <tuple>
#include <utility>

using std::move;
using std::set;
using std::string;
using std::tie;

namespace tcframe {

struct TestCaseData {
    friend class TestCaseDataBuilder;

private:
    string name_;
    string description_;
    set<int> subtaskIds_;

public:
    const string& name() const {
        return name_;
    }

    const string& description() const {
        return description_;
    }

    const set<int>& subtaskIds() const {
        return subtaskIds_;
    }

    bool operator==(const TestCaseData& o) const {
        return tie(name_, description_, subtaskIds_) == tie(o.name_, o.description_, o.subtaskIds_);
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

    TestCaseDataBuilder& setSubtaskIds(set<int> subtaskIds) {
        subject_.subtaskIds_ = subtaskIds;
        return *this;
    }

    TestCaseData build() {
        return move(subject_);
    }
};

}

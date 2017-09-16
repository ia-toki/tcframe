#pragma once

#include <set>
#include <string>
#include <tuple>
#include <utility>

#include "TestCaseData.hpp"
#include "tcframe/util.hpp"

using std::move;
using std::set;
using std::string;
using std::tie;

namespace tcframe {

struct TestCase {
    friend class TestCaseBuilder;

private:
    string name_;
    optional<string> description_;
    set<int> subtaskIds_;
    TestCaseData* data_;

public:
    const string& name() const {
        return name_;
    }

    const optional<string>& description() const {
        return description_;
    }

    const set<int>& subtaskIds() const {
        return subtaskIds_;
    }

    TestCaseData* data() const {
        return data_;
    }

    bool operator==(const TestCase& o) const {
        if (tie(name_, description_, subtaskIds_) != tie(o.name_, o.description_, o.subtaskIds_)) {
            return false;
        }
        if ((data_ == nullptr) != (o.data_ == nullptr)) {
            return false;
        }
        return data_ == nullptr || data_->equals(o.data_);
    }

    static string createName(const string& testGroupName, int testCaseId) {
        return testGroupName + "_" + StringUtils::toString(testCaseId);
    }
};

class TestCaseBuilder {
private:
    TestCase subject_;

public:
    TestCaseBuilder& setName(string name) {
        subject_.name_ = move(name);
        return *this;
    }

    TestCaseBuilder& setDescription(string description) {
        subject_.description_ = optional<string>(move(description));
        return *this;
    }

    TestCaseBuilder& setSubtaskIds(set<int> subtaskIds) {
        subject_.subtaskIds_ = move(subtaskIds);
        return *this;
    }

    TestCaseBuilder& setData(TestCaseData* data) {
        subject_.data_ = data;
        return *this;
    }

    TestCase build() {
        return move(subject_);
    }
};

}

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
    set<int> subtaskIds_;

public:
    SampleTestCase(const string& content, const set<int>& subtaskIds)
              : content_(content)
              , subtaskIds_(subtaskIds) {}

    const string& content() const {
        return content_;
    }

    const set<int>& subtaskIds() const {
        return subtaskIds_;
    }

    bool operator==(const SampleTestCase& o) const {
        return tie(content_, subtaskIds_) == tie(o.content_, subtaskIds_);
    }
};

}

#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace tcframe {

struct SpecificationFailure {
private:
    vector<string> messages_;

public:
    SpecificationFailure(const vector<string>& messages)
            : messages_(messages)
    {}

    const vector<string>& messages() const {
        return messages_;
    }

    bool operator==(const SpecificationFailure& o) const {
        return messages_ == o.messages_;
    }
};

}

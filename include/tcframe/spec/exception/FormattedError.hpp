#pragma once

#include <exception>
#include <string>
#include <utility>
#include <vector>

#include "tcframe/util.hpp"

using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

struct FormattedError : public exception {
private:
    vector<pair<int, string>> messages_;

public:
    ~FormattedError() noexcept = default;

    explicit FormattedError(const vector<pair<int, string>>& messages)
            : messages_(messages) {}

    const vector<pair<int, string>> messages() const {
        return messages_;
    }

    bool operator==(const FormattedError& o) const {
        return messages_ == o.messages_;
    }
};

}

#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::move;
using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

struct FormattedError : public runtime_error {
private:
    vector<pair<int, string>> messages_;

public:
    ~FormattedError() noexcept = default;

    explicit FormattedError(vector<pair<int, string>> messages)
            : runtime_error("")
            , messages_(move(messages)) {}

    const vector<pair<int, string>>& messages() const {
        return messages_;
    }

    bool operator==(const FormattedError& o) const {
        return messages_ == o.messages_;
    }

    static FormattedError combine(const vector<FormattedError>& errors) {
        vector<pair<int, string>> combinedMessages;
        for (const FormattedError& error : errors) {
            combinedMessages.insert(combinedMessages.end(), error.messages().begin(), error.messages().end());
        }
        return FormattedError(combinedMessages);
    }
};

}

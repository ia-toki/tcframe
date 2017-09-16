#pragma once

#include <exception>
#include <functional>
#include <utility>

using std::exception;
using std::function;
using std::move;

namespace tcframe {

struct GenerationException : public exception {
private:
    function<void()> callback_;

public:
    ~GenerationException() noexcept = default;

    explicit GenerationException(function<void()> callback)
            : callback_(move(callback)) {}

    const function<void()>& callback() const {
        return callback_;
    }
};

}

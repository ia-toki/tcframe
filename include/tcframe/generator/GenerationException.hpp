#pragma once

#include <exception>
#include <functional>

using std::exception;
using std::function;

namespace tcframe {

struct GenerationException : public exception {
private:
    function<void()> callback_;

public:
    ~GenerationException() noexcept {}

    GenerationException(const function<void()>& callback)
            : callback_(callback)
    {}

    const function<void()>& callback() const {
        return callback_;
    }
};

}

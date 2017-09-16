#pragma once

namespace tcframe {

/*
 * Partial implementation of std::optional.
 */
template<typename T>
struct optional {
private:
    T* val;

public:
    optional()
            : val(nullptr) {}

    explicit optional(T value)
            : val(new T(value)) {}

    const T& value() const {
        return *val;
    }

    template<typename U>
    T value_or(U&& default_value) const {
        return bool(*this) ? value() : default_value;
    }

    operator bool() const {
        return val != nullptr;
    }

    bool operator==(const optional<T>& other) const {
        if (bool(*this) && bool(other)) {
            return value() == other.value();
        }
        return bool(*this) == bool(other);
    }
};

}

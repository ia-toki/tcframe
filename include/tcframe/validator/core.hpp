#pragma once

#include <functional>
#include <string>
#include <type_traits>
#include <vector>

using std::enable_if_t;
using std::function;
using std::is_arithmetic_v;
using std::size_t;
using std::string;
using std::vector;

namespace tcframe {

template<typename T>
using ScalarType = enable_if_t<is_arithmetic_v<T>>;

template<typename T, typename = ScalarType<T>>
struct ScalarValidator {
private:
    T val;

public:
    explicit ScalarValidator(T _val) : val(_val) {}

    bool isBetween(T minVal, T maxVal) {
        return minVal <= val && val <= maxVal;
    }
};

template<typename T, typename = ScalarType<T>>
inline ScalarValidator<T> valueOf(T val) {
    return ScalarValidator(val);
}

template<typename T, typename = ScalarType<T>>
struct VectorElementValidator {
private:
    const vector<T>& val;

public:
    explicit VectorElementValidator(const vector<T>& _val) : val(_val) {}

    bool isBetween(T minVal, T maxVal) {
        for (T v : val) {
            if (!valueOf(v).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }
    
    bool satisfies(function<bool(T)> predicate) {
        for (T v : val) {
            if (!predicate(v)) {
                return false;
            }
        }
        return true;
    }
};

template<typename T, typename = ScalarType<T>>
inline VectorElementValidator<T> eachElementOf(const vector<T>& val) {
    return VectorElementValidator(val);
}

template<typename T, typename = ScalarType<T>>
struct VectorElementsValidator {
private:
    const vector<T>& val;

public:
    explicit VectorElementsValidator(const vector<T>& _val) : val(_val) {}

    bool areAscending() {
        for (size_t i = 1; i < val.size(); ++i) {
            if (val[i - 1] >= val[i]) {
                return false;
            }
        }
        return true;
    }

    bool areDescending() {
        for (size_t i = 1; i < val.size(); ++i) {
            if (val[i - 1] <= val[i]) {
                return false;
            }
        }
        return true;
    }

    bool areNonAscending() {
        for (size_t i = 1; i < val.size(); ++i) {
            if (val[i - 1] < val[i]) {
                return false;
            }
        }
        return true;
    }

    bool areNonDescending() {
        for (size_t i = 1; i < val.size(); ++i) {
            if (val[i - 1] > val[i]) {
                return false;
            }
        }
        return true;
    }

    bool areUnique() {
        vector<T> v = val;
        sort(v.begin(), v.end());
        size_t ns = unique(v.begin(), v.end()) - v.begin();
        return ns == v.size();
    }
};

template<typename T, typename = ScalarType<T>>
inline VectorElementsValidator<T> elementsOf(const vector<T>& val) {
    return VectorElementsValidator(val);
}

struct StringElementValidator {
private:
    const string& val;

public:
    explicit StringElementValidator(const string& _val) : val(_val) {}

    bool isBetween(char minVal, char maxVal) {
        for (char v : val) {
            if (!valueOf(v).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }
};

inline StringElementValidator eachCharacterOf(const string& val) {
    return StringElementValidator(val);
}

template<typename T, typename = ScalarType<T>>
struct MatrixElementValidator {
private:
    const vector<vector<T>>& val;

public:
    explicit MatrixElementValidator(const vector<vector<T>>& _val) : val(_val) {}

    bool isBetween(T minVal, T maxVal) {
        for (const vector<T>& v : val) {
            if (!eachElementOf(v).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }

    bool satisfies(function<bool(T)> predicate) {
        for (const vector<T>& v : val) {
            if (!eachElementOf(v).satisfies(predicate)) {
                return false;
            }
        }
        return true;
    }
};

template<typename T, typename = ScalarType<T>>
inline MatrixElementValidator<T> eachElementOf(const vector<vector<T>>& val) {
    return MatrixElementValidator(val);
}

}

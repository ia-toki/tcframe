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
    const vector<T>& vec;

public:
    explicit VectorElementValidator(const vector<T>& _vec) : vec(_vec) {}

    bool isBetween(T minVal, T maxVal) {
        for (T e : vec) {
            if (!valueOf(e).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }
    
    bool satisfies(function<bool(T)> predicate) {
        for (T e : vec) {
            if (!predicate(e)) {
                return false;
            }
        }
        return true;
    }
};

template<typename T, typename = ScalarType<T>>
inline VectorElementValidator<T> eachElementOf(const vector<T>& vec) {
    return VectorElementValidator(vec);
}

template<typename T, typename = ScalarType<T>>
struct VectorElementsValidator {
private:
    const vector<T>& vec;

public:
    explicit VectorElementsValidator(const vector<T>& _vec) : vec(_vec) {}

    bool areAscending() {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] >= vec[i]) {
                return false;
            }
        }
        return true;
    }

    bool areDescending() {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] <= vec[i]) {
                return false;
            }
        }
        return true;
    }

    bool areNonAscending() {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] < vec[i]) {
                return false;
            }
        }
        return true;
    }

    bool areNonDescending() {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] > vec[i]) {
                return false;
            }
        }
        return true;
    }

    bool areUnique() {
        vector<T> v = vec;
        sort(v.begin(), v.end());
        size_t ns = unique(v.begin(), v.end()) - v.begin();
        return ns == v.size();
    }
};

template<typename T, typename = ScalarType<T>>
inline VectorElementsValidator<T> elementsOf(const vector<T>& vec) {
    return VectorElementsValidator(vec);
}

struct StringElementValidator {
private:
    const string& str;

public:
    explicit StringElementValidator(const string& _str) : str(_str) {}

    bool isBetween(char minVal, char maxVal) {
        for (char c : str) {
            if (!valueOf(c).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }
};

inline StringElementValidator eachCharacterOf(const string& str) {
    return StringElementValidator(str);
}

template<typename T, typename = ScalarType<T>>
struct MatrixElementValidator {
private:
    const vector<vector<T>>& mat;

public:
    explicit MatrixElementValidator(const vector<vector<T>>& _mat) : mat(_mat) {}

    bool isBetween(T minVal, T maxVal) {
        for (const vector<T>& v : mat) {
            if (!eachElementOf(v).isBetween(minVal, maxVal)) {
                return false;
            }
        }
        return true;
    }

    bool satisfies(function<bool(T)> predicate) {
        for (const vector<T>& v : mat) {
            if (!eachElementOf(v).satisfies(predicate)) {
                return false;
            }
        }
        return true;
    }
};

template<typename T, typename = ScalarType<T>>
inline MatrixElementValidator<T> eachElementOf(const vector<vector<T>>& mat) {
    return MatrixElementValidator(mat);
}

}

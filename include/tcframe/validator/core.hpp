
#include <type_traits>

using std::enable_if_t;
using std::is_arithmetic_v;
using std::size_t;
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
ScalarValidator<T> valueOf(T val) {
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
};

template<typename T, typename = ScalarType<T>>
VectorElementValidator<T> eachElementOf(const vector<T>& val) {
    return VectorElementValidator(val);
}

template<typename T, typename = ScalarType<T>>
VectorElementValidator<T> eachElementOf(vector<T>&& val) {
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
VectorElementsValidator<T> elementsOf(const vector<T>& val) {
    return VectorElementsValidator(val);
}

}

#include <vector>

using std::vector;

namespace tcframe {

template<typename T>
bool eachElementIsBetween(const vector<T>& v, T minVal, T maxVal) {
    for (T x : v) {
        if (x < minVal || x > maxVal) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool elementsAreNonDescending(const vector<T>& v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i - 1] > v[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool elementsAreNonAscending(const vector<T> &v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i - 1] < v[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool elementsAreDescending(const vector<T>& v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i - 1] <= v[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool elementsAreAscending(const vector<T> &v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i - 1] >= v[i]) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool elementsAreUnique(vector<T> v) {
    sort(v.begin(), v.end());
    std::size_t ns = unique(v.begin(), v.end()) - v.begin();
    return ns == v.size();
}

}

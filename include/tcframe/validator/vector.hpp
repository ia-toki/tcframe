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

}

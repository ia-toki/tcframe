
#include <type_traits>

using std::enable_if_t;
using std::is_arithmetic_v;

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

}

#pragma once

#include "VectorSize.hpp"

#include <vector>

using std::vector;

namespace tcframe_old {

template<typename T>
struct VectorWithSize {
public:
    vector<T>* vektor;
    VectorSize size;
};

template<typename T>
VectorWithSize<T> operator%(vector<T>& v, VectorSize size) {
    return VectorWithSize<T>{&v, size};
}

}

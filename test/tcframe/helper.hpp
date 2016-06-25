#pragma once

namespace tcframe {

template<typename T>
struct Captor {
private:
    T* arg_;

public:
    void capture(T arg) {
        arg_ = new T(arg);
    }

    T arg() {
        return *arg_;
    }
};

}

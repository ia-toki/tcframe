#pragma once

namespace tcframe {

template<typename T>
struct Captor {
private:
    T arg_;

public:
    void capture(T arg) {
        this->arg_ = arg;
    }

    T arg() {
        return arg_;
    }
};

}

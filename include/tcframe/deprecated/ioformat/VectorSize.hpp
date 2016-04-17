#pragma once

namespace tcframe_old {

class VectorSize {
public:
    VectorSize(int size)
            : size(size) { }

    int getSize() {
        return size;
    }

private:
    int size;
};

}

#pragma once

#include <functional>

using std::function;

namespace tcframe {

class SeedSetter {
private:
    function<void(unsigned)> setter_;

public:
    virtual ~SeedSetter() {}

    SeedSetter(function<void(unsigned)> setter)
            : setter_(setter) {}

    virtual void setSeed(unsigned seed) {
        setter_(seed);
    }
};

}

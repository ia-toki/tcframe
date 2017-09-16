#pragma once

#include <functional>
#include <utility>

using std::function;
using std::move;

namespace tcframe {

class SeedSetter {
private:
    function<void(unsigned)> setter_;

public:
    virtual ~SeedSetter() = default;

    explicit SeedSetter(function<void(unsigned)> setter)
            : setter_(move(setter)) {}

    virtual void setSeed(unsigned seed) {
        setter_(seed);
    }
};

}

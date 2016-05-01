#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

using std::char_traits;
using std::istream;
using std::ostream;
using std::runtime_error;
using std::string;
using std::tie;

namespace tcframe {

enum VariableType {
    SCALAR,
    VECTOR,
    MATRIX
};

class Variable {
private:
    string name_;
    VariableType type_;

protected:
    Variable(const string& name, VariableType type)
            : name_(name)
            , type_(type)
    {}

public:
    virtual ~Variable() {}

    const string& name() const {
        return name_;
    }

    VariableType type() const {
        return type_;
    }

    bool equals(Variable* o) const {
        return tie(name_, type_) == tie(o->name_, o->type_);
    }
};

}

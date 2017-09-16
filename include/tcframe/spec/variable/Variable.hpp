#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "WhitespaceManipulator.hpp"

using std::char_traits;
using std::enable_if;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::istream;
using std::move;
using std::ostream;
using std::runtime_error;
using std::string;
using std::tie;

namespace tcframe {

template<typename T>
using ScalarCompatible = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using NotScalarCompatible = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

enum class VariableType {
    SCALAR,
    VECTOR,
    MATRIX
};

class Variable {
private:
    string name_;
    VariableType type_;

public:
    virtual ~Variable() = default;

    const string& name() const {
        return name_;
    }

    VariableType type() const {
        return type_;
    }

    bool equals(Variable* o) const {
        return tie(name_, type_) == tie(o->name_, o->type_);
    }

    /* Visible for testing */
    template<typename T>
    static void parseValue(istream* in, T& var, const string& context) {
        WhitespaceManipulator::ensureNoEof(in, context);
        WhitespaceManipulator::ensureNoWhitespace(in, context);

        long long currentPos = in->tellg();
        *in >> var;

        if (in->fail()) {
            in->clear();
            in->seekg(currentPos);
            in->width(20);
            string found;
            *in >> found;

            throw runtime_error("Cannot parse for " + context + ". Found: '" + found + "'");
        }
    }

    /* Visible for testing */
    static void parseRawLine(istream* in, string& var) {
        getline(*in, var);
        if (!in->eof()) {
            in->unget();
        }
    }

protected:
    Variable(string name, VariableType type)
            : name_(move(name))
            , type_(type) {}
};

}

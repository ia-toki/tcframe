#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <vector>

#include "Variable.hpp"
#include "WhitespaceManipulator.hpp"
#include "tcframe/logger.hpp"

using std::iostream;
using std::ostream;
using std::reference_wrapper;
using std::ref;
using std::vector;

namespace tcframe {

class Vector : public Variable {
public:
    virtual ~Vector() {}

    Vector(const string& name)
            : Variable(name, VariableType::VECTOR)
    {}

    virtual int size() const = 0;
    virtual void clear() = 0;
    virtual void printTo(ostream* out) = 0;
    virtual void printElementTo(int index, ostream* out) = 0;
    virtual void parseFrom(istream* in) = 0;
    virtual void parseFrom(istream* in, int size) = 0;
    virtual void parseAndAddElementFrom(istream* in) = 0;

    template<typename T, typename = ScalarCompatible<T>>
    static Vector* create(vector<T>& var, const string& name);
};

template<typename T, typename = ScalarCompatible<T>>
class VectorImpl : public Vector {
private:
    reference_wrapper<vector<T>> var_;

public:
    VectorImpl(vector<T>& var, const string& name)
            : Vector(name)
            , var_(ref(var))
    {}

    int size() const {
        return (int) var_.get().size();
    }

    void clear() {
        var_.get().clear();
    }

    void printTo(ostream* out) {
        bool first = true;
        for (T& element : var_.get()) {
            if (!first) {
                *out << ' ';
            }
            first = false;
            *out << element;
        }
    }

    void printElementTo(int index, ostream* out) {
        *out << var_.get()[index];
    }

    void parseFrom(istream* in) {
        for (int i = 0; ; i++) {
            if (WhitespaceManipulator::canParseNewline(in)) {
                break;
            }
            if (i > 0) {
                WhitespaceManipulator::parseSpaceAfterMissingNewline(
                        in,
                        TokenFormatter::formatVectorElement(name(), i - 1));
            }
            parseAndAddElementFrom(in);
        }
    }

    void parseFrom(istream* in, int size) {
        for (int i = 0; i < size; i++) {
            if (i > 0) {
                WhitespaceManipulator::parseSpace(in, TokenFormatter::formatVectorElement(name(), i - 1));
            }
            parseAndAddElementFrom(in);
        }
    }

    void parseAndAddElementFrom(istream* in) {
        int index = size();
        T element;
        Variable::parseValue(in, element, TokenFormatter::formatVectorElement(name(), index));
        var_.get().push_back(element);
    }
};

template<typename T, typename>
Vector* Vector::create(vector<T>& var, const string& name) {
    return new VectorImpl<T>(var, name);
}

}

#ifndef TCFRAME_TYPE_H
#define TCFRAME_TYPE_H

#include "exception.hpp"

#include <ostream>
#include <sstream>
#include <type_traits>
#include <vector>

using std::enable_if;
using std::integral_constant;
using std::is_arithmetic;
using std::is_same;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class HorizontalVariable {
public:
    virtual ~HorizontalVariable() { }

    virtual void printTo(ostream& out) = 0;
};

template<typename T>
using RequiresScalar = typename enable_if<is_arithmetic<T>::value || is_same<string, T>::value>::type;

template<typename T>
class Scalar : public HorizontalVariable {
public:
    Scalar(T& value) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        out << *value;
    }

private:
    T* value;
};

template<typename T>
class HorizontalVector : public HorizontalVariable {
public:
    HorizontalVector(vector<T>& value) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        bool first = true;
        for (auto element : *value) {
            if (!first) {
                out << " ";
            }
            first = false;
            out << element;
        }
    }

private:
    vector<T>* value;
};

class VerticalVariable {
public:
    virtual ~VerticalVariable() { }

    virtual unsigned int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
};

template<typename T>
class VerticalVector : public VerticalVariable {
public:
    VerticalVector(vector<T>& value) {
        this->value = &value;
    }

    unsigned int size() override {
        return value->size();
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }

private:
    vector<T>* value;
};

class MatrixVariable {
public:
    virtual ~MatrixVariable() { }

    virtual void printTo(ostream& out) = 0;
};

template<typename T>
class Matrix : public MatrixVariable {
public:
    Matrix(vector<vector<T>>& value) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        if (!isValidMatrix()) {
            throw TypeException("Each row of the matrix must have equal number of columns");
        }

        for (auto row : *value) {
            bool first = true;
            for (auto c : row) {
                if (!first) {
                    out << " ";
                }
                first = false;
                out << c;
            }
            out << "\n";
        }
    }

private:
    vector<vector<T>>* value;

    bool isValidMatrix() {
        if (value->empty()) {
            return true;
        }

        unsigned int columnsCount = (*value)[0].size();

        for (auto row : *value) {
            if (row.size() != columnsCount) {
                return false;
            }
        }

        return true;
    }
};

template<>
class Matrix<char> : public MatrixVariable {
public:
    Matrix(vector<vector<char>>& value) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        for (auto line : *value) {
            for (auto c : line) {
                out << c;
            }
            out << "\n";
        }
    }

private:
    vector<vector<char>>* value;
};

}

#endif

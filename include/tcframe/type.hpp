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
    virtual void printTo(ostream& out) = 0;
    virtual ~HorizontalVariable() { };
};

template<typename T>
using RequiresScalar = typename enable_if<is_arithmetic<T>::value || is_same<string, T>::value>::type;

template<typename T>
class Scalar : public HorizontalVariable {
private:
    T* value;

public:
    explicit Scalar(T& value) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        out << *value;
    }
};

template<typename T>
class HorizontalVector : public HorizontalVariable {
private:
    vector<T>* value;

public:
    explicit HorizontalVector(vector<T>& value) {
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
};


class VerticalVariable {
public:
    virtual unsigned int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
};

template<typename T>
class VerticalVector : public VerticalVariable {
private:
    vector<T>* value;

public:
    explicit VerticalVector(vector<T>& value) {
        this->value = &value;
    }

    unsigned int size() override {
        return value->size();
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }
};

class MatrixVariable {
public:
    virtual void printTo(ostream& out) = 0;
    virtual ~MatrixVariable() { }
};

template<typename T>
class Matrix : public MatrixVariable {
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

public:
    explicit Matrix(vector<vector<T>>& value) {
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
};

template<>
class Matrix<char> : public MatrixVariable {
private:
    vector<vector<char>>* value;

public:
    explicit Matrix(vector<vector<char>>& value) {
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
};

}

#endif

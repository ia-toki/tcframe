#ifndef TCFRAME_TYPE_H
#define TCFRAME_TYPE_H

#include "exception.hpp"

#include <ostream>
#include <sstream>
#include <type_traits>
#include <vector>

using std::enable_if;
using std::is_arithmetic;
using std::is_same;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class Variable {
public:
    virtual ~Variable() { }

    string getName() {
        return name;
    }

protected:
    Variable(string name)
            : name(name) { }

private:
    string name;
};

class HorizontalVariable : public Variable {
public:
    virtual ~HorizontalVariable() { }

protected:
    HorizontalVariable(string name)
            : Variable(name) { }
};

class ScalarHorizontalVariable : public HorizontalVariable {
public:
    virtual ~ScalarHorizontalVariable() { }

    virtual void printTo(ostream& out) = 0;

protected:
    ScalarHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

class VectorHorizontalVariable : public HorizontalVariable {
public:
    virtual ~VectorHorizontalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;

protected:
    VectorHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

template<typename T>
using RequiresScalar = typename enable_if<is_arithmetic<T>::value || is_same<string, T>::value>::type;

template<typename T>
class Scalar : public ScalarHorizontalVariable {
public:
    Scalar(T& value, string name)
            : ScalarHorizontalVariable(name) {
        this->value = &value;
    }

    void printTo(ostream& out) override {
        out << *value;
    }

private:
    T* value;
};

template<typename T>
class HorizontalVector : public VectorHorizontalVariable {
public:
    HorizontalVector(vector<T>& value, string name)
            : VectorHorizontalVariable(name) {
        this->value = &value;
    }

    int size() override {
        return (int)value->size();
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }

private:
    vector<T>* value;
};

class VerticalVariable : public Variable {
public:
    virtual ~VerticalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;

protected:
    VerticalVariable(string name)
            : Variable(name) { }
};

template<typename T>
class VerticalVector : public VerticalVariable {
public:
    VerticalVector(vector<T>& value, string name)
            : VerticalVariable(name) {
        this->value = &value;
    }

    int size() override {
        return (int)value->size();
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }

private:
    vector<T>* value;
};

class MatrixVariable : public Variable {
public:
    virtual ~MatrixVariable() { }

    virtual int rowsSize() = 0;
    virtual int columnsSize(int rowIndex) = 0;
    virtual void printElementTo(int rowIndex, int columnIndex, ostream& out) = 0;

protected:
    MatrixVariable(string name)
            : Variable(name) { }
};

template<typename T>
class Matrix : public MatrixVariable {
public:
    Matrix(vector<vector<T>>& value, string name)
            : MatrixVariable(name) {
        this->value = &value;
    }

    int rowsSize() override {
        return (int)value->size();
    }

    int columnsSize(int rowIndex) override {
        return (int)(*value)[rowIndex].size();
    }

    void printElementTo(int rowIndex, int columnIndex, ostream& out) override {
        out << (*value)[rowIndex][columnIndex];
    }

private:
    vector<vector<T>>* value;
};

}

#endif

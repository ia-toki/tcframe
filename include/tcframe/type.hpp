#ifndef TCFRAME_TYPE_H
#define TCFRAME_TYPE_H

#include "exception.hpp"

#include <cctype>
#include <istream>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

using std::char_traits;
using std::enable_if;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

namespace tcframe {

class Variable {
public:
    virtual ~Variable() { }

    virtual void clear() = 0;

    string getName() {
        return name;
    }

protected:
    Variable(string name)
            : name(name) { }

    static void checkNoEof(istream& in, string realName) {
        if (in.peek() == char_traits<char>::eof()) {
            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: <EOF>");
        }
    }

    static void checkNoWhitespace(istream& in, string realName) {
        if (isspace(in.peek())) {
            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: <whitespace>");
        }
    }

    template<typename T>
    static void parseVariableFrom(istream& in, T* value, string realName) {
        checkNoEof(in, realName);
        checkNoWhitespace(in, realName);

        int currentPos = in.tellg();
        in >> *value;

        if (in.fail()) {
            in.clear();
            in.seekg(currentPos);
            in.width(20);
            string found;
            in >> found;

            throw ParsingException("Cannot parse for variable `" + realName + "`. Found: '" + found + "'");
        }
    }

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
    virtual void parseFrom(istream& in) = 0;

protected:
    ScalarHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

class VectorHorizontalVariable : public HorizontalVariable {
public:
    virtual ~VectorHorizontalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
    virtual void parseAndAddElementFrom(istream& in) = 0;

protected:
    VectorHorizontalVariable(string name)
            : HorizontalVariable(name) { }
};

template<typename T>
using RequiresScalar = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using RequiresNotScalar = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

template<typename T>
class Scalar : public ScalarHorizontalVariable {
public:
    Scalar(T& value, string name)
            : ScalarHorizontalVariable(name), value(&value) { }

    void printTo(ostream& out) override {
        out << *value;
    }

    void parseFrom(istream& in) override {
        parseVariableFrom(in, value, getName());
    }

    void clear() override {
        *value = T();
    }

private:
    T* value;
};

template<typename T>
class HorizontalVector : public VectorHorizontalVariable {
public:
    HorizontalVector(vector<T>& value, string name)
            : VectorHorizontalVariable(name), value(&value) { }

    int size() override {
        return (int)(value->size());
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }

    void parseAndAddElementFrom(istream& in) override {
        int index = (int)value->size();
        T element;
        parseVariableFrom(in, &element, getName() + "[" + Util::toString(index) + "]");
        value->push_back(element);
    }

    void clear() override {
        *value = vector<T>();
    }

private:
    vector<T>* value;
};

class VerticalVariable : public Variable {
public:
    virtual ~VerticalVariable() { }

    virtual int size() = 0;
    virtual void printElementTo(int index, ostream& out) = 0;
    virtual void parseAndAddElementFrom(istream& in) = 0;

protected:
    VerticalVariable(string name)
            : Variable(name) { }
};

template<typename T>
class VerticalVector : public VerticalVariable {
public:
    VerticalVector(vector<T>& value, string name)
            : VerticalVariable(name), value(&value) { }

    int size() override {
        return (int)(value->size());
    }

    void printElementTo(int index, ostream& out) override {
        out << (*value)[index];
    }

    void parseAndAddElementFrom(istream& in) override {
        int index = (int)value->size();
        T element;
        parseVariableFrom(in, &element, getName() + "[" + Util::toString(index) + "]");
        value->push_back(element);
    }

    void clear() override {
        *value = vector<T>();
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
    virtual void addRow() = 0;
    virtual void parseAndAddColumnElementFrom(istream& in) = 0;

protected:
    MatrixVariable(string name)
            : Variable(name) { }
};

template<typename T>
class Matrix : public MatrixVariable {
public:
    Matrix(vector<vector<T>>& value, string name)
            : MatrixVariable(name), value(&value) { }

    int rowsSize() override {
        return (int)(value->size());
    }

    int columnsSize(int rowIndex) override {
        return (int)((*value)[rowIndex].size());
    }

    void printElementTo(int rowIndex, int columnIndex, ostream& out) override {
        out << (*value)[rowIndex][columnIndex];
    }

    void addRow() override {
        value->push_back(vector<T>());
    }

    void parseAndAddColumnElementFrom(istream& in) override {
        int rowIndex = (int)value->size() - 1;
        int columnIndex = (int)value->back().size();

        T element;
        parseVariableFrom(in, &element, getName() + "[" + Util::toString(rowIndex) + "]" + "[" + Util::toString(columnIndex) + "]");
        value->back().push_back(element);
    }

    void clear() override {
        *value = vector<vector<T>>();
    }

private:
    vector<vector<T>>* value;
};

}

#endif

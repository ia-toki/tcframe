#pragma once

#include <exception>
#include <iostream>
#include <type_traits>
#include <vector>

#include "TokenFormatter.hpp"
#include "Variable.hpp"
#include "WhitespaceManipulator.hpp"

using std::endl;
using std::iostream;
using std::is_same;
using std::ostream;
using std::vector;

namespace tcframe {

class Matrix : public Variable {
public:
    virtual ~Matrix() = default;

    explicit Matrix(string name)
            : Variable(name, VariableType::MATRIX) {}

    virtual int rows() const = 0;
    virtual int columns(int rowIndex) const = 0;
    virtual void clear() = 0;
    virtual void printTo(ostream* out) = 0;
    virtual void printRowTo(int rowIndex, ostream* out) = 0;
    virtual void parseFrom(istream* in, int rows, int columns) = 0;
    virtual void parseAndAddRowFrom(istream* in, int rowIndex) = 0;

    template<typename T, typename = ScalarCompatible<T>>
    static Matrix* create(vector<vector<T>>& var, string name);
};

template<typename T, typename = ScalarCompatible<T>>
class MatrixImpl : public Matrix {
private:
    vector<vector<T>>* var_;
    bool hasSpaces_;

public:
    virtual ~MatrixImpl() = default;

    MatrixImpl(vector<vector<T>>& var, string name)
            : Matrix(name)
            , var_(&var)
            , hasSpaces_(!is_same<T, char>::value) {}

    int rows() const {
        return (int) var_->size();
    }

    int columns(int rowIndex) const {
        return (int) (*var_)[rowIndex].size();
    }

    void clear() {
        var_->clear();
    }

    void printTo(ostream* out) {
        for (int row = 0; row < var_->size(); row++) {
            printRowTo(row, out);
            *out << endl;
        }
    }

    void printRowTo(int rowIndex, ostream* out) {
        for (int c = 0; c < (*var_)[rowIndex].size(); c++) {
            if (c > 0 && hasSpaces_) {
                *out << ' ';
            }
            *out << (*var_)[rowIndex][c];
        }
    }

    void parseFrom(istream* in, int rows, int columns) {
        for (int r = 0; r < rows; r++) {
            vector<T> row;
            for (int c = 0; c < columns; c++) {
                if (c > 0 && hasSpaces_) {
                    WhitespaceManipulator::parseSpace(in, TokenFormatter::formatMatrixElement(name(), r, c - 1));
                }
                T element;
                Variable::parseValue(in, element, TokenFormatter::formatMatrixElement(name(), r, c));
                row.push_back(element);
            }
            var_->push_back(row);
            WhitespaceManipulator::parseNewline(in, TokenFormatter::formatMatrixElement(name(), r, columns - 1));
        }
    }

    virtual void parseAndAddRowFrom(istream* in, int rowIndex) {
        vector<T> row;
        int c;
        for (c = 0; !WhitespaceManipulator::canParseNewline(in); c++) {
            if (c > 0 && hasSpaces_) {
                WhitespaceManipulator::parseSpaceAfterMissingNewline(
                        in,
                        TokenFormatter::formatMatrixElement(name(), rowIndex, c - 1));
            }
            T element;
            Variable::parseValue(in, element, TokenFormatter::formatMatrixElement(name(), rowIndex, c));
            row.push_back(element);
        }
        var_->push_back(row);
    }
};

template<typename T, typename>
Matrix* Matrix::create(vector<vector<T>>& var, string name) {
    return new MatrixImpl<T>(var, name);
}

}

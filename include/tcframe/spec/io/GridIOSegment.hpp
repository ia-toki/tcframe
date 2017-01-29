#pragma once

#include <functional>
#include <stdexcept>
#include <tuple>

#include "IOSegment.hpp"
#include "tcframe/spec/variable.hpp"

using std::function;
using std::make_tuple;
using std::runtime_error;
using std::tie;

namespace tcframe {

struct GridIOSegment : public IOSegment {
    friend class GridIOSegmentBuilder;

private:
    Matrix* variable_;
    function<int()> rows_;
    function<int()> columns_;

public:
    GridIOSegment()
            : variable_(nullptr)
            , rows_([] {return -1;})
            , columns_([] {return -1;}) {}

    IOSegmentType type() const {
        return IOSegmentType::GRID;
    }

    Matrix* variable() const {
        return variable_;
    }

    const function<int()>& rows() const {
        return rows_;
    }

    const function<int()>& columns() const {
        return columns_;
    }

    bool operator==(const GridIOSegment& o) const {
        if (variable_ != nullptr && o.variable_ != nullptr) {
            if (!variable_->equals(o.variable_)) {
                return false;
            }
        }
        if ((variable_ == nullptr) != (o.variable_ == nullptr)) {
            return false;
        }
        return make_tuple(rows_(), columns_()) == make_tuple(o.rows_(), o.columns_());
    }

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::GRID && *this == *((GridIOSegment*) o);
    }
};

class GridIOSegmentBuilder : public IOSegmentBuilder {
private:
    GridIOSegment* subject_;

public:
    GridIOSegmentBuilder()
            : subject_(new GridIOSegment()) {}

    GridIOSegmentBuilder& addMatrixVariable(Matrix* variable) {
        checkMatrix();
        subject_->variable_ = variable;
        return *this;
    }

    GridIOSegmentBuilder& setSize(function<int()> rows, function<int()> columns) {
        subject_->rows_ = rows;
        subject_->columns_ = columns;
        return *this;
    }

    GridIOSegment* build() {
        checkState();
        return subject_;
    }

private:
    void checkMatrix() {
        if (subject_->variable_ != nullptr) {
            throw runtime_error("Grid segment must have exactly one variable");
        }
    }

    void checkState() {
        if (subject_->variable_ == nullptr) {
            throw runtime_error("Grid segment must have exactly one variable");
        }
        if (subject_->rows_() == -1 || subject_->columns_() == -1) {
            throw runtime_error("Grid segment must define matrix sizes");
        }
    }
};

}

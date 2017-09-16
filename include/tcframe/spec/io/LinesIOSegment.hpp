#pragma once

#include <functional>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

#include "IOSegment.hpp"
#include "tcframe/spec/variable.hpp"

using std::function;
using std::move;
using std::runtime_error;
using std::tie;
using std::vector;

namespace tcframe {

struct LinesIOSegment : public IOSegment {
    friend class LinesIOSegmentBuilder;

private:
    vector<Variable*> variables_;
    function<int()> size_;

public:
    virtual ~LinesIOSegment() = default;

    IOSegmentType type() const {
        return IOSegmentType::LINES;
    }

    const vector<Variable*>& variables() const {
        return variables_;
    }

    const function<int()>& size() const {
        return size_;
    }

    bool operator==(const LinesIOSegment& o) const {
        if (size_() != o.size_()) {
            return false;
        }
        if (variables_.size() != o.variables_.size()) {
            return false;
        }
        for (int i = 0; i < variables_.size(); i++) {
            if (!variables_[i]->equals(o.variables_[i])) {
                return false;
            }
        }
        return true;
    }

    bool equals(IOSegment* o) const {
        return o->type() == IOSegmentType::LINES && *this == *((LinesIOSegment*) o);
    }
};

class LinesIOSegmentBuilder : public IOSegmentBuilder {
private:
    LinesIOSegment *subject_ = new LinesIOSegment();

public:
    LinesIOSegmentBuilder() {
        subject_->size_ = [] {return NO_SIZE;};
    }

    LinesIOSegmentBuilder& addVectorVariable(Vector* variable) {
        checkJaggedVector();
        subject_->variables_.push_back(variable);
        return *this;
    }

    LinesIOSegmentBuilder& addJaggedVectorVariable(Matrix* variable) {
        checkJaggedVector();
        subject_->variables_.push_back(variable);
        return *this;
    }

    LinesIOSegmentBuilder& setSize(function<int()> size) {
        subject_->size_ = move(size);
        return *this;
    }

    LinesIOSegment* build() {
        checkState();
        return move(subject_);
    }

private:
    void checkState() {
        if (subject_->variables_.empty()) {
            throw runtime_error("Lines segment must have at least one variable");
        }
    }

    void checkJaggedVector() {
        if (!subject_->variables_.empty() && subject_->variables_.back()->type() == VariableType::MATRIX) {
            throw runtime_error("Jagged vector can only be the last variable in a lines segment");
        }
    }
};

}

#pragma once

#include <stdexcept>
#include <tuple>
#include <vector>

#include "IOSegment.hpp"
#include "tcframe/spec/variable.hpp"

using std::runtime_error;
using std::tie;
using std::vector;

namespace tcframe {

struct LinesIOSegment : public IOSegment {
    friend class LinesIOSegmentBuilder;

private:
    vector<Variable*> variables_;
    int size_;

public:
    IOSegmentType type() const {
        return IOSegmentType::LINES;
    }

    const vector<Variable*>& variables() const {
        return variables_;
    }

    int size() const {
        return size_;
    }

    bool operator==(const LinesIOSegment& o) const {
        if (size_ != o.size_) {
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
    LinesIOSegment *subject_;

public:
    LinesIOSegmentBuilder()
            : subject_(new LinesIOSegment()) {
        subject_->size_ = -1;
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

    LinesIOSegmentBuilder& setSize(int size) {
        subject_->size_ = size;
        return *this;
    }

    LinesIOSegment* build() {
        checkState();
        return subject_;
    }

private:
    void checkState() {
        if (subject_->variables_.empty()) {
            throw runtime_error("Lines segment must have at least one variable");
        }

        if (subject_->size_ == -1) {
            throw runtime_error("Lines segment must define vector sizes");
        }
    }

    void checkJaggedVector() {
        if (!subject_->variables_.empty() && subject_->variables_.back()->type() == VariableType::MATRIX) {
            throw runtime_error("Jagged vector can only be the last variable in a lines segment");
        }
    }
};

}

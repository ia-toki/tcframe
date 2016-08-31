#pragma once

#include <algorithm>
#include <queue>
#include <string>
#include <vector>

#include "tcframe/logger.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::queue;
using std::string;
using std::vector;

#define CONS(predicate) addConstraint([=] {return predicate;}, #predicate)
#define CASE(...) addOfficialTestCase([=] {__VA_ARGS__;}, #__VA_ARGS__)
#define EMPTY_LINE() MagicLineIOSegmentBuilder(newLineIOSegment(), "")
#define LINE(...) MagicLineIOSegmentBuilder(newLineIOSegment(), #__VA_ARGS__), __VA_ARGS__
#define LINES(...) (MagicLinesIOSegmentBuilder(newLinesIOSegment(), #__VA_ARGS__), __VA_ARGS__)
#define GRID(...) (MagicGridIOSegmentBuilder(newGridIOSegment(), #__VA_ARGS__), __VA_ARGS__)

#define SIZE_IMPL1(size) VectorSize{size}
#define SIZE_IMPL2(rows, columns) MatrixSize{rows, columns}
#define SIZE_WITH_COUNT(_1, _2, N, ...) SIZE_IMPL ## N
#define SIZE(...) SIZE_WITH_COUNT(__VA_ARGS__, 2, 1)(__VA_ARGS__)


namespace tcframe {

struct VectorSize {
    int size;
};

template<typename T>
struct VectorWithSize {
    vector<T>* vektor;
    VectorSize size;
};

template<typename T>
VectorWithSize<T> operator%(vector<T>& vektor, VectorSize size) {
    return VectorWithSize<T>{&vektor, size};
}

struct MatrixSize {
    int rows;
    int columns;
};

class VariableNamesExtractor {
private:
    queue<string> names_;

public:
    VariableNamesExtractor(string names) {
        for (string name : extractVariableNames(names)) {
            names_.push(name);
        }
    }

    string nextName() {
        string name = names_.front();
        names_.pop();
        return name;
    }

private:
    static vector<string> extractVariableNames(const string& s) {
        vector<string> names;
        for (string namePossiblyWithSize : StringUtils::split(s, ',')) {
            vector<string> tokens = StringUtils::splitAndTrimBySpace(namePossiblyWithSize);
            names.push_back(tokens[0]);
        }
        return names;
    }
};

class MagicLineIOSegmentBuilder {
private:
    LineIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicLineIOSegmentBuilder(LineIOSegmentBuilder& builder, string names)
            : builder_(&builder)
            , extractor_(VariableNamesExtractor(names)) {}

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(T& var) {
        builder_->addScalarVariable(Scalar::create(var, extractor_.nextName()));
        return *this;
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(vector<T>& var) {
        builder_->addVectorVariable(Vector::create(var, extractor_.nextName()));
        return *this;
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(VectorWithSize<T> var) {
        builder_->addVectorVariable(Vector::create(*var.vektor, extractor_.nextName()), var.size.size);
        return *this;
    }

    template<typename T, typename = NotScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a line segment");
    }
};

class MagicLinesIOSegmentBuilder {
private:
    LinesIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicLinesIOSegmentBuilder(LinesIOSegmentBuilder& builder, string names)
            : builder_(&builder)
            , extractor_(VariableNamesExtractor(names)) {}

    template<typename T, typename = ScalarCompatible<T>>
    MagicLinesIOSegmentBuilder& operator,(vector<T>& var) {
        builder_->addVectorVariable(Vector::create(var, extractor_.nextName()));
        return *this;
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLinesIOSegmentBuilder& operator,(vector<vector<T>>& var) {
        builder_->addJaggedVectorVariable(Matrix::create(var, extractor_.nextName()));
        return *this;
    }

    template<typename T>
    MagicLinesIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a lines segment");
    }

    MagicLinesIOSegmentBuilder& operator%(VectorSize size) {
        builder_->setSize(size.size);
        return *this;
    }
};

class MagicGridIOSegmentBuilder {
private:
    GridIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicGridIOSegmentBuilder(GridIOSegmentBuilder& builder, string names)
            : builder_(&builder)
            , extractor_(VariableNamesExtractor(names)) {}

    template<typename T, typename = ScalarCompatible<T>>
    MagicGridIOSegmentBuilder& operator,(vector<vector<T>>& var) {
        builder_->addMatrixVariable(Matrix::create(var, extractor_.nextName()));
        return *this;
    }

    template<typename T>
    MagicLineIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a grid segment");
    }

    MagicGridIOSegmentBuilder& operator%(MatrixSize size) {
        builder_->setSize(size.rows, size.columns);
        return *this;
    }
};

}

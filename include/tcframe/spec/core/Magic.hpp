#pragma once

#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "tcframe/spec/io.hpp"
#include "tcframe/spec/variable.hpp"
#include "tcframe/util.hpp"

using std::function;
using std::move;
using std::queue;
using std::string;
using std::vector;

#define CONS(predicate) addConstraint([=] {return predicate;}, #predicate)
#define CASE(...) addOfficialTestCase([=] {__VA_ARGS__;}, #__VA_ARGS__)
#define EMPTY_LINE() MagicLineIOSegmentBuilder(newLineIOSegment(), "")
#define LINE(...) (MagicLineIOSegmentBuilder(newLineIOSegment(), #__VA_ARGS__), __VA_ARGS__)
#define LINES(...) (MagicLinesIOSegmentBuilder(newLinesIOSegment(), #__VA_ARGS__), __VA_ARGS__)
#define RAW_LINE(...) (MagicRawLineIOSegmentBuilder(newRawLineIOSegment(), #__VA_ARGS__), __VA_ARGS__)
#define RAW_LINES(...) (MagicRawLinesIOSegmentBuilder(newRawLinesIOSegment(), #__VA_ARGS__), __VA_ARGS__)
#define GRID(...) (MagicGridIOSegmentBuilder(newGridIOSegment(), #__VA_ARGS__), __VA_ARGS__)

#define SIZE_IMPL1(size) VectorSize([=] {return size;})
#define SIZE_IMPL2(rows, columns) MatrixSize([=] {return rows;}, [=] {return columns;})
#define SIZE_WITH_COUNT(_1, _2, N, ...) SIZE_IMPL ## N
#define SIZE(...) SIZE_WITH_COUNT(__VA_ARGS__, 2, 1)(__VA_ARGS__)


namespace tcframe {

struct VectorSize {
    function<int()> size;

    explicit VectorSize(function<int()> size)
            : size(move(size)) {}
};

template<typename T>
struct VectorWithSize {
    vector<T>* vektor;
    VectorSize size;
};

template<typename T>
VectorWithSize<T> operator%(vector<T>& vektor, VectorSize size) {
    return {&vektor, size};
}

struct MatrixSize {
    function<int()> rows;
    function<int()> columns;

    MatrixSize(function<int()> rows, function<int()> columns)
            : rows(move(rows))
            , columns(move(columns)) {}
};

class VariableNamesExtractor {
private:
    queue<string> names_;

public:
    explicit VariableNamesExtractor(const string& names) {
        for (const string& name : extractVariableNames(names)) {
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
        for (const string& namePossiblyWithSize : StringUtils::split(s, ',')) {
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
    MagicLineIOSegmentBuilder(LineIOSegmentBuilder& builder, const string& names)
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
    MagicLineIOSegmentBuilder& operator,(const VectorWithSize<T>& var) {
        builder_->addVectorVariable(Vector::create(*var.vektor, extractor_.nextName()), var.size.size);
        return *this;
    }

    template<typename T, typename = NotScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a line segment");
    }

    MagicLineIOSegmentBuilder& operator%(const VectorSize&) {
        throw runtime_error("Specifying size is not allowed after a line segment");
    }

    MagicLineIOSegmentBuilder& operator%(const MatrixSize&) {
        throw runtime_error("Specifying size is not allowed after a line segment");
    }
};

class MagicLinesIOSegmentBuilder {
private:
    LinesIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicLinesIOSegmentBuilder(LinesIOSegmentBuilder& builder, const string& names)
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

    MagicLinesIOSegmentBuilder& operator%(const VectorSize& size) {
        builder_->setSize(size.size);
        return *this;
    }
};

class MagicRawLineIOSegmentBuilder {
private:
    RawLineIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicRawLineIOSegmentBuilder(RawLineIOSegmentBuilder& builder, const string& names)
            : builder_(&builder)
            , extractor_(VariableNamesExtractor(names)) {}

    MagicRawLineIOSegmentBuilder& operator,(string& var) {
        builder_->addScalarVariable(Scalar::createRaw(var, extractor_.nextName()));
        return *this;
    }

    template<typename T>
    MagicRawLineIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a raw line segment");
    }

    MagicRawLineIOSegmentBuilder& operator%(const VectorSize&) {
        throw runtime_error("Specifying size is not allowed after a raw line segment");
    }

    MagicRawLineIOSegmentBuilder& operator%(const MatrixSize&) {
        throw runtime_error("Specifying size is not allowed after a raw line segment");
    }
};

class MagicRawLinesIOSegmentBuilder {
private:
    RawLinesIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicRawLinesIOSegmentBuilder(RawLinesIOSegmentBuilder& builder, const string& names)
            : builder_(&builder)
            , extractor_(VariableNamesExtractor(names)) {}

    MagicRawLinesIOSegmentBuilder& operator,(vector<string>& var) {
        builder_->addVectorVariable(Vector::createRaw(var, extractor_.nextName()));
        return *this;
    }

    template<typename T>
    MagicRawLinesIOSegmentBuilder& operator,(T var) {
        throw runtime_error(
                "The type of variable " + TokenFormatter::formatVariable(extractor_.nextName())
                + " is not supported for a raw lines segment");
    }

    MagicRawLinesIOSegmentBuilder& operator%(const VectorSize& size) {
        builder_->setSize(size.size);
        return *this;
    }
};

class MagicGridIOSegmentBuilder {
private:
    GridIOSegmentBuilder* builder_;
    VariableNamesExtractor extractor_;

public:
    MagicGridIOSegmentBuilder(GridIOSegmentBuilder& builder, const string& names)
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

    MagicGridIOSegmentBuilder& operator%(const MatrixSize& size) {
        builder_->setSize(size.rows, size.columns);
        return *this;
    }
};

}

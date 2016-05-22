#pragma once

#include <algorithm>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>

#include "tcframe/io.hpp"
#include "tcframe/util.hpp"
#include "tcframe/variable.hpp"

using std::enable_if;
using std::forward;
using std::is_arithmetic;
using std::is_reference;
using std::is_same;
using std::queue;
using std::string;
using std::vector;

#define CONS(predicate) addConstraint(Constraint([=] {return predicate;}, #predicate))
#define CASE(...) addOfficialTestCase(OfficialTestCase([=] {__VA_ARGS__;}, #__VA_ARGS__))
#define SAMPLE_CASE(...) addSampleTestCase(__VA_ARGS__)
#define LINE(...) MagicLineIOSegmentBuilder(newLineIOSegment(), #__VA_ARGS__), __VA_ARGS__

#define SIZE_IMPL1(size) VectorSize{size}
#define SIZE_WITH_COUNT(_1, _2, N, ...) SIZE_IMPL ## N
#define SIZE(...) SIZE_WITH_COUNT(__VA_ARGS__, 2, 1)(__VA_ARGS__)


namespace tcframe {

template<typename T>
using ScalarCompatible = typename enable_if<!is_reference<T>::value && (is_arithmetic<T>::value || is_same<string, T>::value)>::type;

template<typename T>
using NotScalarCompatible = typename enable_if<is_reference<T>::value || (!is_arithmetic<T>::value && !is_same<string, T>::value)>::type;

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

class MagicLineIOSegmentBuilder {
private:
    LineIOSegmentBuilder* builder_;
    queue<string> names_;

public:
    MagicLineIOSegmentBuilder(LineIOSegmentBuilder& builder, string names) {
        builder_ = &builder;
        for (string name : extractVariableNames(names)) {
            names_.push(name);
        }
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(T& var) {
        builder_->addScalarVariable(Scalar::create(var, nextName()));
        return *this;
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(vector<T>& var) {
        builder_->addVectorVariable(Vector::create(var, nextName()));
        return *this;
    }

    template<typename T, typename = ScalarCompatible<T>>
    MagicLineIOSegmentBuilder& operator,(VectorWithSize<T> var) {
        builder_->addVectorVariable(Vector::create(*var.vektor, nextName()), var.size.size);
        return *this;
    }

private:
    string nextName() {
        string name = names_.front();
        names_.pop();
        return name;
    }

    static vector<string> extractVariableNames(const string& s) {
        vector<string> names;
        for (string namePossiblyWithSize : StringUtils::split(s, ',')) {
            vector<string> tokens = StringUtils::splitAndTrimBySpace(namePossiblyWithSize);
            names.push_back(tokens[0]);
        }
        return names;
    }

};

}

#ifndef TCFRAME_TCFRAME_H
#define TCFRAME_TCFRAME_H

#include "generator.hpp"

#define EMPTY_LINE() addLineSegment("")
#define LINE(...) addLineSegment(#__VA_ARGS__), __VA_ARGS__
#define LINES(...) addLinesSegment(#__VA_ARGS__), __VA_ARGS__
#define GRID(...) addGridSegment(#__VA_ARGS__), __VA_ARGS__

#define ESCAPED_CASE(escaped_closure) addTestCase([this] {escaped_closure;}, #escaped_closure)
#define CASE(...) ESCAPED_CASE((__VA_ARGS__))
#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif

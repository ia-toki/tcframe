#ifndef TCFRAME_TCFRAME_H
#define TCFRAME_TCFRAME_H

#include "generator.h"

#define ESCAPED_CASE(escaped_closure) addTestCase([this] {escaped_closure;}, #escaped_closure)
#define CASE(...) ESCAPED_CASE((__VA_ARGS__))
#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif

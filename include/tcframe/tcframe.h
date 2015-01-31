#ifndef TCFRAME_TCFRAME_H
#define TCFRAME_TCFRAME_H

#include "generator.h"

#define CASE(closure) addTestCase([this] {closure}, #closure)
#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif

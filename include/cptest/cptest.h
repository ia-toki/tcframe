#ifndef CPTEST_CPTEST_H
#define CPTEST_CPTEST_H

#include "generator.h"

#define CASE(closure) addTestCase([this] {closure}, #closure)
#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif

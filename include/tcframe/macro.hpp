#ifndef TCFRAME_MACRO_H
#define TCFRAME_MACRO_H

#define SIZE_IMPL1(size) VectorSize(size)
#define SIZE_IMPL2(rowsSize, columnsSize) MatrixSizes(rowsSize, columnsSize)
#define SIZE_WITH_COUNT(_1, _2, N, ...) SIZE_IMPL ## N
#define SIZE(...) SIZE_WITH_COUNT(__VA_ARGS__, 2, 1)(__VA_ARGS__)

#define EMPTY_LINE() addLineSegment("")
#define LINE(...) addLineSegment(#__VA_ARGS__), __VA_ARGS__
#define LINES(...) (addLinesSegment(#__VA_ARGS__), __VA_ARGS__)
#define GRID(...) (addGridSegment(#__VA_ARGS__), __VA_ARGS__)

#define ESCAPED_CASE(escaped_closure) addOfficialTestCase([this] {escaped_closure;}, #escaped_closure)
#define CASE(...) ESCAPED_CASE((__VA_ARGS__))
#define SAMPLE_CASE(...) addSampleTestCase(__VA_ARGS__)

#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif
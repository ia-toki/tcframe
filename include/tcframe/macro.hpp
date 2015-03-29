#ifndef TCFRAME_MACRO_H
#define TCFRAME_MACRO_H

#define SIZE_IMPL1(size) VectorSize(size)
#define SIZE_IMPL2(rowsSize, columnsSize) MatrixSizes(rowsSize, columnsSize)
#define SIZE_WITH_COUNT(_1, _2, N, ...) SIZE_IMPL ## N
#define SIZE(...) SIZE_WITH_COUNT(__VA_ARGS__, 2, 1)(__VA_ARGS__)

#define EMPTY_LINE() applyLastSegment(), applyLineSegment("")
#define LINE(...) applyLastSegment(), (applyLineSegment(#__VA_ARGS__), __VA_ARGS__)
#define LINES(...) applyLastSegment(), (applyLinesSegment(#__VA_ARGS__), __VA_ARGS__)
#define GRID(...) applyLastSegment(), (applyGridSegment(#__VA_ARGS__), __VA_ARGS__)

#define CASE(...) addOfficialTestCase([this] {__VA_ARGS__;}, #__VA_ARGS__)
#define SAMPLE_CASE(...) addSampleTestCase(__VA_ARGS__)

#define CONS(predicate) addConstraint([this] {return predicate;}, #predicate)

#endif
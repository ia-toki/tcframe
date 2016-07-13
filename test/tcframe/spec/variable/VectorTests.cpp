#include "gmock/gmock.h"

#include "tcframe/spec/variable/Vector.hpp"

#include <sstream>

using std::istringstream;
using std::ostringstream;

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class VectorTests : public Test {
protected:
    vector<int> v;
    Vector* V = Vector::create(v, "V");
};

TEST_F(VectorTests, Parsing_Successful) {
    istringstream in("1 2 3");
    V->parseFrom(&in, 3);

    EXPECT_THAT(v, Eq(vector<int>{1, 2, 3}));
}

TEST_F(VectorTests, Parsing_Failed_MissingSpace) {
    istringstream in("1 2");

    try {
        V->parseFrom(&in, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Expected: <space> after 'V[1]'"));
    }
}

TEST_F(VectorTests, Parsing_Failed_LeadingWhitepace) {
    istringstream in(" 1 2 3");

    try {
        V->parseFrom(&in, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'V[0]'. Found: <whitespace>"));
    }
}

TEST_F(VectorTests, Parsing_Failed_ExtraWhitepace) {
    istringstream in("1 2  3");

    try {
        V->parseFrom(&in, 3);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'V[2]'. Found: <whitespace>"));
    }
}

TEST_F(VectorTests, Parsing_WithoutSize_Successful) {
    istringstream in("1 2 3\n");
    V->parseFrom(&in);

    EXPECT_THAT(v, Eq(vector<int>{1, 2, 3}));
}

TEST_F(VectorTests, Parsing_WithoutSize_Failed_LeadingWhitespace) {
    istringstream in(" 1 2 3");

    try {
        V->parseFrom(&in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'V[0]'. Found: <whitespace>"));
    }
}

TEST_F(VectorTests, Parsing_WithoutSize_Failed_MissingVariable) {
    istringstream in("1 2  3");

    try {
        V->parseFrom(&in);
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("Cannot parse for 'V[2]'. Found: <whitespace>"));
    }
}

TEST_F(VectorTests, Printing_WithoutSize) {
    v = {1, 2, 3};
    ostringstream out;
    V->printTo(&out);

    EXPECT_THAT(out.str(), Eq("1 2 3"));
}

TEST_F(VectorTests, Printing_Elements) {
    v = {1, 2, 3};
    ostringstream out;
    V->printElementTo(0, &out);
    out << ' ';
    V->printElementTo(1, &out);
    out << ' ';
    V->printElementTo(2, &out);

    EXPECT_THAT(out.str(), Eq("1 2 3"));
}

}

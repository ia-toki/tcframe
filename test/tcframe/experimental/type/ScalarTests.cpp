#include "gmock/gmock.h"

#include "tcframe/experimental/type/Scalar.hpp"

#include <sstream>
#include <string>
#include <vector>

using std::istringstream;
using std::ostringstream;
using std::string;
using std::vector;

using ::testing::Eq;
using ::testing::Test;

using tcframe::experimental::Scalar;

class ExperimentalScalarTest : public Test {
protected:
    // This is a SFINAE trick that checks whether Scalar(var, string()) is a legal expression.
    // If it is legal, then the first overload will be selected (checkCanBeScalar(T& var, int)).
    // The second parameter is really necessary so that we can have 2 overloads for a single
    //     checkCanBeScalar(var, 0) expression.
    template<typename T>
    static bool canBeScalar(T &var) {
        return checkCanBeScalar(var, 0);
    }

    class Foo {};

private:
    template<typename T>
    static auto checkCanBeScalar(T& var, int) -> decltype(Scalar(var, string()), bool()) {
        return true;
    }

    template<typename T>
    static bool checkCanBeScalar(T& var, long) {
        return false;
    }
};

TEST_F(ExperimentalScalarTest, CanConstructOnSupportedTypes) {
    int int_;
    unsigned int unsigned_int_;
    short short_;
    unsigned short unsigned_short_;
    char char_;
    unsigned char unsigned_char_;
    long long_;
    unsigned long unsigned_long_;
    long long long_long_;
    unsigned long long unsigned_long_long_;
    float float_;
    double double_;
    long double long_double_;
    string string_;

    EXPECT_TRUE(canBeScalar(int_));
    EXPECT_TRUE(canBeScalar(unsigned_int_));
    EXPECT_TRUE(canBeScalar(short_));
    EXPECT_TRUE(canBeScalar(unsigned_short_));
    EXPECT_TRUE(canBeScalar(char_));
    EXPECT_TRUE(canBeScalar(unsigned_char_));
    EXPECT_TRUE(canBeScalar(long_));
    EXPECT_TRUE(canBeScalar(unsigned_long_));
    EXPECT_TRUE(canBeScalar(long_long_));
    EXPECT_TRUE(canBeScalar(unsigned_long_long_));
    EXPECT_TRUE(canBeScalar(float_));
    EXPECT_TRUE(canBeScalar(double_));
    EXPECT_TRUE(canBeScalar(long_double_));
    EXPECT_TRUE(canBeScalar(string_));
}

TEST_F(ExperimentalScalarTest, CannotConstructOnUnsupportedTypes){
    vector<int> vector_int_;
    int* int_ptr;
    int*& int_ptr_ref = int_ptr;
    void (*func)() = []() {};
    Foo foo;

    EXPECT_FALSE(canBeScalar(vector_int_));
    EXPECT_FALSE(canBeScalar(int_ptr));
    EXPECT_FALSE(canBeScalar(int_ptr_ref));
    EXPECT_FALSE(canBeScalar(func));
    EXPECT_FALSE(canBeScalar(foo));
}

TEST_F(ExperimentalScalarTest, CanAccessName) {
    int a;
    Scalar A(a, "a");

    EXPECT_THAT(A.name(), Eq("a"));
}

TEST_F(ExperimentalScalarTest, CanUseParseFunction) {
    int a;
    Scalar A(a, "a");

    istringstream sin("42");
    A.parseFunction()(sin);

    EXPECT_THAT(a, Eq(42));
}

TEST_F(ExperimentalScalarTest, CanUsePrintFunction) {
    int a = 42;
    Scalar A(a, "a");

    ostringstream sout;
    A.printFunction()(sout);

    EXPECT_THAT(sout.str(), Eq("42"));
}

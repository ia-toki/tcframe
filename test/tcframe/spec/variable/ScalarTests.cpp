#include "gmock/gmock.h"

#include "tcframe/spec/variable/Scalar.hpp"

#include <vector>
#include <sstream>

using ::testing::Eq;
using ::testing::Test;

using std::istringstream;
using std::ostringstream;
using std::vector;

namespace tcframe {

class ScalarTests : public Test {
protected:
    int a;
    Scalar* A = Scalar::create(a, "a");

    /* This is a SFINAE trick that checks whether Scalar::create(var, string()) is a legal expression.
       If it is legal, then the first overload will be selected.
       If it is not, then the second overload will be selected.
    */
    template<typename T>
    static bool canBeScalar(T& var) {
        return checkCanBeScalar(var, 0);
    }

    class Foo {};

private:
    template<typename T>
    static auto checkCanBeScalar(T& var, int) -> decltype(Scalar::create(var, string()), bool()) {
        return true;
    }

    template<typename T>
    static bool checkCanBeScalar(T& var, long) {
        return false;
    }
};

TEST_F(ScalarTests, Compatibility_SupportedTypes) {
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

TEST_F(ScalarTests, Compatibility_UnsupportedTypes){
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

TEST_F(ScalarTests, Parsing) {
    istringstream in("42");
    A->parseFrom(&in);
    EXPECT_THAT(a, Eq(42));
}

TEST_F(ScalarTests, Printing) {
    ostringstream out;
    a = 42;
    A->printTo(&out);
    EXPECT_THAT(out.str(), Eq("42"));
}

}

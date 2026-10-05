#include "gmock/gmock.h"

#include "BaseEteTests.cpp"

using ::testing::AllOf;
using ::testing::Eq;
using ::testing::HasSubstr;
using ::testing::Test;

namespace tcframe {

class FunctionalEteTests : public BaseEteTests {};

// The spec is compiled directly (the Python build still passes a single --solution, see SPEC.md T6.4).
static const char* BUILD_SPEC =
        "cd test-ete/functional && mkdir -p build && "
        "g++ -std=c++17 -D__TCFRAME_SPEC_FILE__=\"\\\"$PWD/spec.cpp\\\"\" -I ../../tcframe/include "
        "-o build/spec ../../tcframe/src/tcframe/runner.cpp";

TEST_F(FunctionalEteTests, Generate_AndGrade_CorrectSolutions) {
    ASSERT_THAT(execStatus(BUILD_SPEC), Eq(0));

    exec("cd test-ete/functional && build/spec --output=build/tc "
         "--solution-file=encoder=ref/encoder.cpp --solution-file=decoder=ref/decoder.cpp");
    EXPECT_THAT(exec("ls test-ete/functional/build/tc"), AllOf(HasSubstr("functional_1.in"), HasSubstr("functional_1.out")));

    string result = exec(
            "cd test-ete/functional && build/spec grade --output=build/tc "
            "--solution-file=encoder=ref/encoder.cpp --solution-file=decoder=ref/decoder.cpp");
    EXPECT_THAT(result, AllOf(
            HasSubstr("functional_1: Accepted"),
            HasSubstr("functional_2: Accepted"),
            HasSubstr("functional_3: Accepted"),
            HasSubstr("Accepted [100]")));
}

TEST_F(FunctionalEteTests, Grade_WrongDecoder) {
    ASSERT_THAT(execStatus(BUILD_SPEC), Eq(0));

    string result = exec(
            "cd test-ete/functional && build/spec grade --output=build/tc "
            "--solution-file=encoder=ref/encoder.cpp --solution-file=decoder=wrong/decoder.cpp");
    EXPECT_THAT(result, AllOf(
            HasSubstr("functional_1: Wrong Answer"),
            HasSubstr("Wrong Answer")));
}

TEST_F(FunctionalEteTests, Grade_MissingKeyIsError) {
    ASSERT_THAT(execStatus(BUILD_SPEC), Eq(0));

    string result = exec(
            "cd test-ete/functional && build/spec grade --output=build/tc --solution-file=encoder=ref/encoder.cpp 2>&1; echo status=$?");
    EXPECT_THAT(result, AllOf(
            HasSubstr("solution keys mismatch"),
            HasSubstr("status=1")));
}


// Build and run through the registry scripts (SPEC.md T6.3): build_cpp / run_cpp.
TEST_F(FunctionalEteTests, Grade_CustomCppScripts) {
    ASSERT_THAT(execStatus(BUILD_SPEC), Eq(0));
    ASSERT_THAT(execStatus(
            "cd test-ete/functional && build/spec --output=build/tc "
            "--evaluator-dir=../../tcframe/registry/evaluators/functional --solution-family=cpp "
            "--solution-file=encoder=ref/encoder.cpp --solution-file=decoder=ref/decoder.cpp > /dev/null"), Eq(0));

    string result = exec(
            "cd test-ete/functional && build/spec grade --output=build/tc "
            "--evaluator-dir=../../tcframe/registry/evaluators/functional --solution-family=cpp "
            "--solution-file=encoder=ref/encoder.cpp --solution-file=decoder=ref/decoder.cpp");
    EXPECT_THAT(result, AllOf(
            HasSubstr("functional_1: Accepted"),
            HasSubstr("Accepted [100]")));
}

// Pascal functional solutions through build_pascal / run_pascal (FPC). Needs fpc on PATH.
static const char* BUILD_PASCAL_SPEC =
        "cd test-ete/functional-pascal && mkdir -p build && "
        "g++ -std=c++17 -D__TCFRAME_SPEC_FILE__=\"\\\"$PWD/spec.cpp\\\"\" -I ../../tcframe/include "
        "-o build/spec ../../tcframe/src/tcframe/runner.cpp";

static const char* PASCAL_GRADE =
        "cd test-ete/functional-pascal && build/spec grade --output=build/tc "
        "--evaluator-dir=../../tcframe/registry/evaluators/functional --solution-family=pascal ";

static const char* PASCAL_GENERATE =
        "cd test-ete/functional-pascal && build/spec --output=build/tc "
        "--evaluator-dir=../../tcframe/registry/evaluators/functional --solution-family=pascal "
        "--solution-file=encoder=ref/encoder.pas --solution-file=decoder=ref/decoder.pas";

TEST_F(FunctionalEteTests, Grade_Pascal_CorrectSolutions) {
    ASSERT_THAT(execStatus(BUILD_PASCAL_SPEC), Eq(0));
    ASSERT_THAT(execStatus(string(PASCAL_GENERATE) + " > /dev/null"), Eq(0));

    string result = exec(string(PASCAL_GRADE) +
            "--solution-file=encoder=ref/encoder.pas --solution-file=decoder=ref/decoder.pas");
    EXPECT_THAT(result, AllOf(
            HasSubstr("_1: Accepted"),
            HasSubstr("Accepted [100]")));
}

TEST_F(FunctionalEteTests, Grade_Pascal_WrongDecoder) {
    ASSERT_THAT(execStatus(BUILD_PASCAL_SPEC), Eq(0));
    ASSERT_THAT(execStatus(string(PASCAL_GENERATE) + " > /dev/null"), Eq(0));

    string result = exec(string(PASCAL_GRADE) +
            "--solution-file=encoder=ref/encoder.pas --solution-file=decoder=wrong/decoder.pas");
    EXPECT_THAT(result, HasSubstr("Wrong Answer"));
}

}

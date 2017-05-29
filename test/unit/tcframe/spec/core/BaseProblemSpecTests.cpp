#include "gmock/gmock.h"

#include "tcframe/spec/core/BaseProblemSpec.hpp"

using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Property;
using ::testing::SizeIs;
using ::testing::StrEq;
using ::testing::Test;

namespace tcframe {

class BaseProblemSpecTests : public Test {
protected:
    class ProblemSpec : public BaseProblemSpec {
    protected:
        int T;
        int A, B;
        vector<int> X;
        vector<vector<int>> M;

        void InputFormat() {
            newLineIOSegment()
                    .addScalarVariable(Scalar::create(A, "A"))
                    .addScalarVariable(Scalar::create(B, "B"));
            newLinesIOSegment()
                    .addVectorVariable(Vector::create(X, "X"))
                    .setSize([] {return 3;});
        }
    };

    class ProblemSpecWithOutputFormat : public ProblemSpec {
    public:
        int Z;

    protected:
        void BeforeOutputFormat() {
            Z = 42;
        }

        void OutputFormat() {
            newGridIOSegment()
                    .addMatrixVariable(Matrix::create(M, "M"))
                    .setSize([] {return 2;}, [] {return 3;})
                    .build();
        }
    };

    class ProblemSpecWithMultipleOutputFormats : public ProblemSpec {
    protected:
        void OutputFormat1() {
            newLineIOSegment()
                    .addScalarVariable(Scalar::create(A, "A"))
                    .addScalarVariable(Scalar::create(B, "B"));
            newLinesIOSegment()
                    .addVectorVariable(Vector::create(X, "X"))
                    .setSize([] {return 3;});
        }

        void OutputFormat2() {
            newGridIOSegment()
                    .addMatrixVariable(Matrix::create(M, "M"))
                    .setSize([] {return 2;}, [] {return 3;})
                    .build();
        }
    };

    class ProblemSpecWithInvalidOutputFormats : public ProblemSpecWithOutputFormat {
    protected:
        void OutputFormat1() {}
        void OutputFormat2() {}
    };

    class ProblemSpecWithStyleConfig : public ProblemSpec {
    protected:
        void StyleConfig() {
            CustomScorer();
            NoOutput();
        }
    };

    class ProblemSpecWithMultipleTestCasesConfig : public ProblemSpec {
    protected:
        void MultipleTestCasesConfig() {
            Counter(T);
            OutputPrefix("Case #%d: ");
        }
    };

    class ProblemSpecWithGradingConfig : public ProblemSpec {
    protected:
        void GradingConfig() {
            TimeLimit(3);
            MemoryLimit(128);
        }
    };

    class ProblemSpecWithMultipleTestCasesConstraints : public ProblemSpec {
    protected:
        int T;

        void MultipleTestCasesConstraints() {
            addConstraint([=] {return 1 <= T && T <= 20;}, "1 <= T && T <= 20");
        }
    };

    class ProblemSpecWithConstraints : public ProblemSpec {
    protected:
        void Constraints() {
            addConstraint([=] {return 1 <= A && A <= 100;}, "1 <= A && A <= 100");
            addConstraint([=] {return 1 <= B && B <= 100;}, "1 <= B && B <= 100");
        }
    };

    class ProblemSpecWithSubtasks : public ProblemSpecWithConstraints {
    protected:
        void Subtask1() {
            Points(70.0);

            addConstraint([=] {return 1 <= A && A <= 100;}, "1 <= A && A <= 100");
            addConstraint([=] {return 1 <= B && B <= 100;}, "1 <= B && B <= 100");
            addConstraint([=] {return A != B;}, "A != B");
        }

        void Subtask2() {
            Points(30.0);

            addConstraint([=] {return 1 <= A && A <= 1000;}, "1 <= A && A <= 1000");
            addConstraint([=] {return 1 <= B && B <= 1000;}, "1 <= B && B <= 1000");
        }
    };
};

TEST_F(BaseProblemSpecTests, StyleConfig) {
    StyleConfig config = ProblemSpecWithStyleConfig().buildStyleConfig();
    EXPECT_TRUE(config.needsCustomScorer());
    EXPECT_FALSE(config.needsOutput());
}

TEST_F(BaseProblemSpecTests, MultipleTestCasesConfig) {
    MultipleTestCasesConfig config = ProblemSpecWithMultipleTestCasesConfig().buildMultipleTestCasesConfig();
    EXPECT_TRUE(config.counter());
    EXPECT_THAT(config.outputPrefix(), Eq(optional<string>("Case #%d: ")));
}

TEST_F(BaseProblemSpecTests, GradingConfig) {
    GradingConfig config = ProblemSpecWithGradingConfig().buildGradingConfig();
    EXPECT_THAT(config.timeLimit(), Eq(3));
    EXPECT_THAT(config.memoryLimit(), Eq(128));
}

TEST_F(BaseProblemSpecTests, IOFormat) {
    ProblemSpecWithOutputFormat problemSpec;
    problemSpec.Z = 0;

    IOFormat ioFormat = problemSpec.buildIOFormat();
    EXPECT_THAT(ioFormat.inputFormat(), SizeIs(2));
    ASSERT_THAT(ioFormat.outputFormats(), SizeIs(1));
    EXPECT_THAT(ioFormat.outputFormats()[0], SizeIs(1));

    ioFormat.beforeOutputFormat()();
    EXPECT_THAT(problemSpec.Z, Eq(42));
}

TEST_F(BaseProblemSpecTests, IOFormat_MultipleOutputFormats) {
    IOFormat ioFormat = ProblemSpecWithMultipleOutputFormats().buildIOFormat();
    ASSERT_THAT(ioFormat.outputFormats(), SizeIs(2));
    EXPECT_THAT(ioFormat.outputFormats()[0], SizeIs(2));
    EXPECT_THAT(ioFormat.outputFormats()[1], SizeIs(1));
}

TEST_F(BaseProblemSpecTests, IOFormat_MultipleOutputFormats_Invalid) {
    try {
        ProblemSpecWithInvalidOutputFormats().buildIOFormat();
        FAIL();
    } catch (runtime_error& e) {
        EXPECT_THAT(e.what(), StrEq("If OutputFormat() is specified, no other OutputFormatX() can be specified"));
    }
}

TEST_F(BaseProblemSpecTests, Constraints) {
    ConstraintSuite constraintSuite = ProblemSpecWithConstraints().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
        AllOf(Property(&Subtask::id, Subtask::MAIN_ID), Property(&Subtask::constraints, SizeIs(2)))));
}

TEST_F(BaseProblemSpecTests, MultipleTestCasesConstraints) {
    ConstraintSuite constraintSuite = ProblemSpecWithMultipleTestCasesConstraints().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.multipleTestCasesConstraints(), SizeIs(1));
}

TEST_F(BaseProblemSpecTests, Subtasks) {
    ConstraintSuite constraintSuite = ProblemSpecWithSubtasks().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            AllOf(Property(&Subtask::id, Subtask::MAIN_ID), Property(&Subtask::constraints, SizeIs(2))),
            AllOf(Property(&Subtask::id, 1), Property(&Subtask::constraints, SizeIs(3))),
            AllOf(Property(&Subtask::id, 2), Property(&Subtask::constraints, SizeIs(2)))));
}

}

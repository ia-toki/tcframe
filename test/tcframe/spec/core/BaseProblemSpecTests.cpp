#include "gmock/gmock.h"

#include "tcframe/experimental/runner.hpp"

using ::testing::A;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::Eq;
using ::testing::Property;
using ::testing::SizeIs;
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
                    .setSize(3);
            newGridIOSegment()
                    .addMatrixVariable(Matrix::create(M, "M"))
                    .setSize(2, 3)
                    .build();
        }

        void OutputFormat() {
            newLinesIOSegment()
                    .addVectorVariable(Vector::create(X, "X"))
                    .setSize(3);
            newGridIOSegment()
                    .addMatrixVariable(Matrix::create(M, "M"))
                    .setSize(2, 3)
                    .build();
            newLineIOSegment()
                    .addScalarVariable(Scalar::create(A, "A"))
                    .addScalarVariable(Scalar::create(B, "B"));
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

    class ProblemSpecWithSubtasks : public ProblemSpec {
    protected:
        void Subtask1() {
            addConstraint([=] {return 1 <= A && A <= 100;}, "1 <= A && A <= 100");
            addConstraint([=] {return 1 <= B && B <= 100;}, "1 <= B && B <= 100");
            addConstraint([=] {return A != B;}, "A != B");
        }

        void Subtask2() {
            addConstraint([=] {return 1 <= A && A <= 1000;}, "1 <= A && A <= 1000");
            addConstraint([=] {return 1 <= B && B <= 1000;}, "1 <= B && B <= 1000");
        }
    };
};

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
    IOFormat ioFormat = ProblemSpec().buildIOFormat();
    EXPECT_THAT(ioFormat.inputFormat(), ElementsAre(
        A<LineIOSegment*>(),
        A<LinesIOSegment*>(),
        A<GridIOSegment*>()));
    EXPECT_THAT(ioFormat.outputFormat(), ElementsAre(
            A<LinesIOSegment*>(),
            A<GridIOSegment*>(),
            A<LineIOSegment*>()));
}

TEST_F(BaseProblemSpecTests, Constraints) {
    ConstraintSuite constraintSuite = ProblemSpecWithConstraints().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
        AllOf(Property(&Subtask::id, -1), Property(&Subtask::constraints, SizeIs(2)))));
}

TEST_F(BaseProblemSpecTests, MultipleTestCasesConstraints) {
    ConstraintSuite constraintSuite = ProblemSpecWithMultipleTestCasesConstraints().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.multipleTestCasesConstraints(), SizeIs(1));
}

TEST_F(BaseProblemSpecTests, Subtasks) {
    ConstraintSuite constraintSuite = ProblemSpecWithSubtasks().buildConstraintSuite();
    EXPECT_THAT(constraintSuite.constraints(), ElementsAre(
            AllOf(Property(&Subtask::id, 1), Property(&Subtask::constraints, SizeIs(3))),
            AllOf(Property(&Subtask::id, 2), Property(&Subtask::constraints, SizeIs(2)))));
}

}

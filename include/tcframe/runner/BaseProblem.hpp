#pragma once

#include <vector>

#include "NotImplementedException.hpp"
#include "tcframe/config.hpp"
#include "tcframe/constraint.hpp"
#include "tcframe/format.hpp"

using std::vector;

namespace tcframe {

class BaseProblem : protected ProblemConfigBuilder, protected IOFormatBuilder, protected ConstraintSuiteBuilder {
private:
    vector<void(BaseProblem::*)()> subtasks_ = {
            &BaseProblem::Subtask1,
            &BaseProblem::Subtask2,
            &BaseProblem::Subtask3,
            &BaseProblem::Subtask4,
            &BaseProblem::Subtask5,
            &BaseProblem::Subtask6,
            &BaseProblem::Subtask7,
            &BaseProblem::Subtask8,
            &BaseProblem::Subtask9,
            &BaseProblem::Subtask10};

public:
    virtual ~BaseProblem() {}

    virtual ProblemConfig buildProblemConfig() {
        applyProblemConfig();
        return ProblemConfigBuilder::build();
    }

    IOFormat buildIOFormat() {
        prepareForInputFormat();
        InputFormat();
        return IOFormatBuilder::build();
    }

    ConstraintSuite buildConstraintSuite() {
        try {
            Constraints();
            return ConstraintSuiteBuilder::build();
        } catch (NotImplementedException&) {
            for (auto subtask : subtasks_) {
                try {
                    ConstraintSuiteBuilder::newConstraintGroup();
                    (this->*subtask)();
                } catch (NotImplementedException&) {
                    break;
                }
            }
            return ConstraintSuiteBuilder::buildWithoutLastConstraintGroup();
        }
    }

protected:
    virtual void applyProblemConfig() {
        Config();
    }

    virtual void Config() {}
    virtual void InputFormat() = 0;
    virtual void Constraints() {throw NotImplementedException();}
    virtual void Subtask1() {throw NotImplementedException();}
    virtual void Subtask2() {throw NotImplementedException();}
    virtual void Subtask3() {throw NotImplementedException();}
    virtual void Subtask4() {throw NotImplementedException();}
    virtual void Subtask5() {throw NotImplementedException();}
    virtual void Subtask6() {throw NotImplementedException();}
    virtual void Subtask7() {throw NotImplementedException();}
    virtual void Subtask8() {throw NotImplementedException();}
    virtual void Subtask9() {throw NotImplementedException();}
    virtual void Subtask10() {throw NotImplementedException();}
};

}

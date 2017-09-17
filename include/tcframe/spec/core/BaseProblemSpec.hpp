#pragma once

#include <vector>

#include "tcframe/exception.hpp"
#include "tcframe/spec/config.hpp"
#include "tcframe/spec/constraint.hpp"
#include "tcframe/spec/io.hpp"

using std::vector;

namespace tcframe {

class BaseProblemSpec
        : protected IOFormatBuilder,
          protected StyleConfigBuilder,
          protected GradingConfigBuilder,
          protected MultipleTestCasesConfigBuilder,
          protected ConstraintSuiteBuilder {
private:
    vector<void(BaseProblemSpec::*)()> outputFormats_ = {
            &BaseProblemSpec::OutputFormat1,
            &BaseProblemSpec::OutputFormat2,
            &BaseProblemSpec::OutputFormat3,
            &BaseProblemSpec::OutputFormat4,
            &BaseProblemSpec::OutputFormat5};

    vector<void(BaseProblemSpec::*)()> subtasks_ = {
            &BaseProblemSpec::Subtask1,
            &BaseProblemSpec::Subtask2,
            &BaseProblemSpec::Subtask3,
            &BaseProblemSpec::Subtask4,
            &BaseProblemSpec::Subtask5,
            &BaseProblemSpec::Subtask6,
            &BaseProblemSpec::Subtask7,
            &BaseProblemSpec::Subtask8,
            &BaseProblemSpec::Subtask9,
            &BaseProblemSpec::Subtask10,
            &BaseProblemSpec::Subtask11,
            &BaseProblemSpec::Subtask12,
            &BaseProblemSpec::Subtask13,
            &BaseProblemSpec::Subtask14,
            &BaseProblemSpec::Subtask15,
            &BaseProblemSpec::Subtask16,
            &BaseProblemSpec::Subtask17,
            &BaseProblemSpec::Subtask18,
            &BaseProblemSpec::Subtask19,
            &BaseProblemSpec::Subtask20,
            &BaseProblemSpec::Subtask21,
            &BaseProblemSpec::Subtask22,
            &BaseProblemSpec::Subtask23,
            &BaseProblemSpec::Subtask24,
            &BaseProblemSpec::Subtask25};

public:
    virtual ~BaseProblemSpec() = default;

    IOFormat buildIOFormat() {
        IOFormatBuilder::prepareForInputFormat();
        InputFormat();
        IOFormatBuilder::setBeforeOutputFormat([this] {
            BeforeOutputFormat();
        });
        try {
            IOFormatBuilder::newOutputFormat();
            OutputFormat();
            for (auto outputFormat : outputFormats_) {
                try {
                    (this->*outputFormat)();
                    throw runtime_error("If OutputFormat() is specified, no other OutputFormatX() can be specified");
                } catch (NotImplementedException&) {}
            }
        } catch (NotImplementedException&) {
            for (auto outputFormat : outputFormats_) {
                try {
                    (this->*outputFormat)();
                    IOFormatBuilder::newOutputFormat();
                } catch (NotImplementedException&) {
                    break;
                }
            }
        }
        return IOFormatBuilder::build();
    }

    tcframe::StyleConfig buildStyleConfig() {
        StyleConfig();
        return StyleConfigBuilder::build();
    }

    tcframe::GradingConfig buildGradingConfig() {
        GradingConfig();
        return GradingConfigBuilder::build();
    }

    tcframe::MultipleTestCasesConfig buildMultipleTestCasesConfig() {
        MultipleTestCasesConfig();
        return MultipleTestCasesConfigBuilder::build();
    }

    ConstraintSuite buildConstraintSuite() {
        Constraints();
        for (auto subtask : subtasks_) {
            try {
                ConstraintSuiteBuilder::newSubtask();
                (this->*subtask)();
            } catch (NotImplementedException&) {
                break;
            }
        }

        ConstraintSuiteBuilder::prepareForMultipleTestCasesConstraints();
        MultipleTestCasesConstraints();
        return ConstraintSuiteBuilder::build();
    }

protected:
    virtual void InputFormat() = 0;
    virtual void BeforeOutputFormat() {}
    virtual void OutputFormat() {throw NotImplementedException();}
    virtual void OutputFormat1() {throw NotImplementedException();}
    virtual void OutputFormat2() {throw NotImplementedException();}
    virtual void OutputFormat3() {throw NotImplementedException();}
    virtual void OutputFormat4() {throw NotImplementedException();}
    virtual void OutputFormat5() {throw NotImplementedException();}
    virtual void StyleConfig() {}
    virtual void GradingConfig() {}
    virtual void MultipleTestCasesConfig() {}
    virtual void MultipleTestCasesConstraints() {}
    virtual void Constraints() {}
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
    virtual void Subtask11() {throw NotImplementedException();}
    virtual void Subtask12() {throw NotImplementedException();}
    virtual void Subtask13() {throw NotImplementedException();}
    virtual void Subtask14() {throw NotImplementedException();}
    virtual void Subtask15() {throw NotImplementedException();}
    virtual void Subtask16() {throw NotImplementedException();}
    virtual void Subtask17() {throw NotImplementedException();}
    virtual void Subtask18() {throw NotImplementedException();}
    virtual void Subtask19() {throw NotImplementedException();}
    virtual void Subtask20() {throw NotImplementedException();}
    virtual void Subtask21() {throw NotImplementedException();}
    virtual void Subtask22() {throw NotImplementedException();}
    virtual void Subtask23() {throw NotImplementedException();}
    virtual void Subtask24() {throw NotImplementedException();}
    virtual void Subtask25() {throw NotImplementedException();}
};

}

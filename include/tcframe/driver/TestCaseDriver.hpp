#pragma once

#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "RawIOManipulator.hpp"
#include "tcframe/exception.hpp"
#include "tcframe/spec/config.hpp"
#include "tcframe/spec/io.hpp"
#include "tcframe/spec/testcase.hpp"
#include "tcframe/spec/verifier.hpp"
#include "tcframe/util.hpp"

using std::endl;
using std::istream;
using std::move;
using std::ostream;
using std::runtime_error;
using std::string;

namespace tcframe {

class TestCaseDriver {
private:
    RawIOManipulator* rawIOManipulator_;
    IOManipulator* ioManipulator_;
    Verifier* verifier_;
    MultipleTestCasesConfig multipleTestCasesConfig_;

public:
    virtual ~TestCaseDriver() = default;

    TestCaseDriver(
            RawIOManipulator* rawIOManipulator,
            IOManipulator* ioManipulator,
            Verifier* verifier,
            MultipleTestCasesConfig multipleTestCasesConfig)
            : ioManipulator_(ioManipulator)
            , rawIOManipulator_(rawIOManipulator)
            , verifier_(verifier)
            , multipleTestCasesConfig_(move(multipleTestCasesConfig)) {}

    virtual void generateInput(const TestCase& testCase, ostream* out) {
        applyInput(testCase);
        verifyInput(testCase);
        writeInput(testCase, out);
    }

    virtual void generateSampleOutput(const TestCase& testCase, ostream* out) {
        applySampleOutput(testCase);
        writeSampleOutput(testCase, out);
    }

    virtual void validateOutput(istream* in) {
        applyOutput(in);
    }

    virtual void validateMultipleTestCasesInput(int testCaseCount) {
        *multipleTestCasesConfig_.counter().value() = testCaseCount;

        MultipleTestCasesConstraintsVerificationResult result = verifier_->verifyMultipleTestCasesConstraints();
        if (!result.isValid()) {
            throw result.asFormattedError();
        }
    }

private:
    void applyInput(const TestCase& testCase) {
        if (testCase.data()->type() == TestCaseDataType::OFFICIAL) {
            auto data = (OfficialTestCaseData*) testCase.data();
            data->closure()();
        } else {
            auto data = (SampleTestCaseData*) testCase.data();
            istringstream in(data->input());
            ioManipulator_->parseInput(&in);
        }
    }

    void verifyInput(const TestCase& testCase) {
        ConstraintsVerificationResult result = verifier_->verifyConstraints(testCase.subtaskIds());
        if (!result.isValid()) {
            throw result.asFormattedError();
        }
    }

    void writeInput(const TestCase& testCase, ostream* out) {
        if (multipleTestCasesConfig_.counter()) {
            int testCaseCount = 1;
            rawIOManipulator_->printLine(out, StringUtils::toString(testCaseCount));
        }

        if (testCase.data()->type() == TestCaseDataType::SAMPLE) {
            auto data = (SampleTestCaseData*) testCase.data();
            rawIOManipulator_->print(out, data->input());
        } else {
            ioManipulator_->printInput(out);
        }
    }

    void applySampleOutput(const TestCase& testCase) {
        auto data = (SampleTestCaseData*) testCase.data();
        istringstream in(data->output().value());
        ioManipulator_->parseOutput(&in);
    }

    void writeSampleOutput(const TestCase& testCase, ostream* out) {
        if (multipleTestCasesConfig_.counter() && multipleTestCasesConfig_.outputPrefix()) {
            string firstOutputPrefix = StringUtils::interpolate(multipleTestCasesConfig_.outputPrefix().value(), 1);
            rawIOManipulator_->print(out, firstOutputPrefix);
        }

        auto data = (SampleTestCaseData*) testCase.data();
        rawIOManipulator_->print(out, data->output().value());
    }

    void applyOutput(istream* in) {
        if (multipleTestCasesConfig_.counter() && multipleTestCasesConfig_.outputPrefix()) {
            string firstOutputPrefix = StringUtils::interpolate(multipleTestCasesConfig_.outputPrefix().value(), 1);
            for (char p : firstOutputPrefix) {
                int c = in->peek();
                if (c == char_traits<char>::eof() || (char) c != p) {
                    throw runtime_error("Output must start with \"" + firstOutputPrefix + "\"");
                }
                in->get();
            }
        }

        ioManipulator_->parseOutput(in);
    }
};

}

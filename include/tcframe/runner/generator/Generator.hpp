#pragma once

#include <sstream>
#include <string>

#include "GenerationOptions.hpp"
#include "GeneratorLogger.hpp"
#include "TestCaseGenerator.hpp"
#include "tcframe/exception.hpp"
#include "tcframe/runner/client.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/util.hpp"

using std::ostringstream;
using std::string;

namespace tcframe {

class Generator {
private:
    SpecClient* specClient_;
    TestCaseGenerator* testCaseGenerator_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~Generator() = default;

    Generator(
            SpecClient* specClient,
            TestCaseGenerator* testCaseGenerator,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : specClient_(specClient)
            , testCaseGenerator_(testCaseGenerator)
            , os_(os)
            , logger_(logger) {}

    virtual bool generate(const GenerationOptions& options) {
        logger_->logIntroduction();

        TestSuite testSuite = specClient_->getTestSuite();
        bool hasMultipleTestCases = specClient_->hasMultipleTestCases();
        optional<string> multipleTestCasesOutputPrefix = specClient_->getMultipleTestCasesOutputPrefix();

        specClient_->setSeed(options.seed());

        os_->forceMakeDir(options.outputDir());

        bool successful = true;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            successful &= generateTestGroup(testGroup, options, hasMultipleTestCases, multipleTestCasesOutputPrefix);
        }
        if (successful) {
            logger_->logSuccessfulResult();
        } else {
            logger_->logFailedResult();
        }
        return successful;
    }

private:
    bool generateTestGroup(
            const TestGroup& testGroup,
            const GenerationOptions& options,
            bool hasMultipleTestCases,
            optional<string> multipleTestCasesOutputPrefix) {

        logger_->logTestGroupIntroduction(testGroup.id());

        bool successful = true;
        for (const TestCase& testCase : testGroup.testCases()) {
            successful &= testCaseGenerator_->generate(testCase, options);
        }
        if (successful && hasMultipleTestCases && !testGroup.testCases().empty()) {
            return combineMultipleTestCases(testGroup, options, multipleTestCasesOutputPrefix);
        }
        return successful;
    }

    bool combineMultipleTestCases(
            const TestGroup& testGroup,
            const GenerationOptions& options,
            optional<string> multipleTestCasesOutputPrefix) {

        string testGroupName = TestGroup::createName(options.slug(), testGroup.id());
        logger_->logMultipleTestCasesCombinationIntroduction(testGroupName);

        try {
            verify(testGroup);
            combine(testGroup, options, multipleTestCasesOutputPrefix);
        } catch (runtime_error& e) {
            logger_->logMultipleTestCasesCombinationFailedResult();
            logger_->logError(&e);
            return false;
        }

        logger_->logMultipleTestCasesCombinationSuccessfulResult();
        return true;
    }

    void verify(const TestGroup& testGroup) {
        specClient_->validateMultipleTestCasesInput((int) testGroup.testCases().size());
    }

    void combine(
            const TestGroup& testGroup,
            const GenerationOptions& options,
            optional<string> multipleTestCasesOutputPrefix) {

        int testCaseCount = (int) testGroup.testCases().size();

        string testGroupName = TestGroup::createName(options.slug(), testGroup.id());
        string testGroupIn = TestCasePathCreator::createInputPath(testGroupName, options.outputDir());
        string testGroupOut = TestCasePathCreator::createOutputPath(testGroupName, options.outputDir());
        bool hasTcOutput = options.hasTcOutput();

        ostringstream sout;

        sout << "echo " << testCaseCount << " > " << testGroupIn;

        if (hasTcOutput) {
            sout << " && touch " << testGroupOut;
        }

        os_->execute(ExecutionRequestBuilder().setCommand(sout.str()).build());

        for (int i = 1; i <= testCaseCount; i++) {
            string testCaseName = TestCase::createName(testGroupName, i);
            string in = TestCasePathCreator::createInputPath(testCaseName, options.outputDir());
            string out = TestCasePathCreator::createOutputPath(testCaseName, options.outputDir());

            ostringstream sout2;
            sout2 << "tail -n +2 " << in << " >> " << testGroupIn;

            if (hasTcOutput) {
                sout2 << " && ";
                if (i > 1 && multipleTestCasesOutputPrefix) {
                    string outputPrefix = multipleTestCasesOutputPrefix.value();
                    // Replace the prefix for the first tc, with the correct prefix for this tc
                    string firstPrefix = StringUtils::interpolate(outputPrefix, 1);
                    string correctPrefix = StringUtils::interpolate(outputPrefix, i);
                    sout2 << "printf \"%b\" \"" << escapeForBash(correctPrefix) << "\" >> " << testGroupOut << " && ";
                    sout2 << "tail -c +" << (firstPrefix.size() + 1) << " " << out << " >> " << testGroupOut;
                } else {
                    sout2 << "cat " << out << " >> " << testGroupOut;
                }
            }

            os_->execute(ExecutionRequestBuilder().setCommand(sout2.str()).build());

            ostringstream sout3;
            sout3 << "rm " << in << " " << out;
            os_->execute(ExecutionRequestBuilder().setCommand(sout3.str()).build());
        }
    }

    static string escapeForBash(const string& s) {
        string res = s;
        res = StringUtils::replace(res, '\\', "\\\\");
        res = StringUtils::replace(res, '$', "\\$");
        res = StringUtils::replace(res, '"', "\\\"");
        res = StringUtils::replace(res, '\n', "\\n");
        res = StringUtils::replace(res, '\t', "\\t");
        return res;
    }
};

class GeneratorFactory {
public:
    virtual ~GeneratorFactory() = default;

    virtual Generator* create(
            SpecClient* specClient,
            TestCaseGenerator* testCaseGenerator,
            OperatingSystem* os,
            GeneratorLogger* logger) {

        return new Generator(specClient, testCaseGenerator, os, logger);
    }
};

}

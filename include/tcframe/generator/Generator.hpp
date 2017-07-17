#pragma once

#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "GenerationOptions.hpp"
#include "GeneratorLogger.hpp"
#include "TestCaseGenerator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec/core.hpp"

using std::function;
using std::istringstream;
using std::vector;
using std::set;
using std::string;

namespace tcframe {

class Generator {
private:
    SeedSetter* seedSetter_;
    TestCaseGenerator* testCaseGenerator_;
    Verifier* verifier_;
    OperatingSystem* os_;
    GeneratorLogger* logger_;

public:
    virtual ~Generator() {}

    Generator(
            SeedSetter* seedSetter,
            TestCaseGenerator* testCaseGenerator,
            Verifier* verifier,
            OperatingSystem* os,
            GeneratorLogger* logger)
            : seedSetter_(seedSetter)
            , testCaseGenerator_(testCaseGenerator)
            , verifier_(verifier)
            , os_(os)
            , logger_(logger) {}

    virtual bool generate(const TestSuite& testSuite, const GenerationOptions& options) {
        logger_->logIntroduction();

        seedSetter_->setSeed(options.seed());

        os_->forceMakeDir(options.outputDir());

        bool successful = true;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            successful &= generateTestGroup(testGroup, options);
        }
        if (successful) {
            logger_->logSuccessfulResult();
        } else {
            logger_->logFailedResult();
        }
        return successful;
    }

private:
    bool generateTestGroup(const TestGroup& testGroup, const GenerationOptions& options) {
        logger_->logTestGroupIntroduction(testGroup.id());

        bool successful = true;
        for (const TestCase& testCase : testGroup.testCases()) {
            successful &= testCaseGenerator_->generate(testCase, options);
        }
        if (successful && options.multipleTestCasesCounter() != nullptr && !testGroup.testCases().empty()) {
            return combineMultipleTestCases(testGroup, options);
        }
        return successful;
    }

    bool combineMultipleTestCases(const TestGroup& testGroup, const GenerationOptions& options) {
        string testGroupName = TestGroup::createName(options.slug(), testGroup.id());
        logger_->logMultipleTestCasesCombinationIntroduction(testGroupName);

        *options.multipleTestCasesCounter() = (int) testGroup.testCases().size();

        try {
            verify();
            combine(testGroup, options);
        } catch (GenerationException& e) {
            logger_->logMultipleTestCasesCombinationFailedResult();
            e.callback()();
            return false;
        } catch (runtime_error& e) {
            logger_->logMultipleTestCasesCombinationFailedResult();
            logger_->logSimpleFailure(e.what());
            return false;
        }

        logger_->logMultipleTestCasesCombinationSuccessfulResult();
        return true;
    }

    void verify() {
        MultipleTestCasesConstraintsVerificationResult result = verifier_->verifyMultipleTestCasesConstraints();
        if (!result.isValid()) {
            throw GenerationException([=] {logger_->logMultipleTestCasesConstraintsVerificationFailure(result);});
        }
    }

    void combine(const TestGroup& testGroup, const GenerationOptions& options) {
        int testCaseCount = (int) testGroup.testCases().size();

        string testGroupName = TestGroup::createName(options.slug(), testGroup.id());
        string testGroupIn = options.outputDir() + "/" + testGroupName + ".in";
        string testGroupOut = options.outputDir() + "/" + testGroupName + ".out";
        bool needsOutput = options.needsOutput();

        ostringstream sout;

        sout << "echo " << testCaseCount << " > " << testGroupIn;

        if (needsOutput) {
            sout << " && touch " << testGroupOut;
        }

        os_->execute(ExecutionRequestBuilder().setCommand(sout.str()).build());

        for (int i = 1; i <= testCaseCount; i++) {
            string testCaseName = TestCase::createName(testGroupName, i);
            string in = options.outputDir() + "/" + testCaseName + ".in";
            string out = options.outputDir() + "/" + testCaseName + ".out";

            ostringstream sout2;
            sout2 << "tail -n +2 " << in << " >> " << testGroupIn;

            if (needsOutput) {
                sout2 << " && ";
                if (i > 1 && options.multipleTestCasesOutputPrefix()) {
                    string outputPrefix = options.multipleTestCasesOutputPrefix().value();
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
    virtual ~GeneratorFactory() {}

    virtual Generator* create(
            SeedSetter* seedSetter,
            TestCaseGenerator* testCaseGenerator,
            Verifier* verifier,
            OperatingSystem* os,
            GeneratorLogger* logger) {

        return new Generator(seedSetter, testCaseGenerator, verifier, os, logger);
    }
};

}

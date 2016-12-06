#pragma once

#include <functional>
#include <vector>
#include <set>
#include <sstream>
#include <string>

#include "GeneratorLogger.hpp"
#include "TestCaseGenerator.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec/core.hpp"
#include "tcframe/verifier.hpp"

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

    virtual bool generate(const TestSuite& testSuite, const GeneratorConfig& config) {
        logger_->logIntroduction();

        seedSetter_->setSeed(config.seed());

        os_->forceMakeDir(config.outputDir());

        bool successful = true;
        for (const TestGroup& testGroup : testSuite.testGroups()) {
            successful &= generateTestGroup(testGroup, config);
        }
        if (successful) {
            logger_->logSuccessfulResult();
        } else {
            logger_->logFailedResult();
        }
        return successful;
    }

private:
    bool generateTestGroup(const TestGroup& testGroup, const GeneratorConfig& config) {
        logger_->logTestGroupIntroduction(testGroup.id());

        bool successful = true;
        for (const TestCase& testCase : testGroup.testCases()) {
            successful &= testCaseGenerator_->generate(testCase, config);
        }
        if (successful && config.multipleTestCasesCounter() != nullptr && !testGroup.testCases().empty()) {
            return combineMultipleTestCases(testGroup, config);
        }
        return successful;
    }

    bool combineMultipleTestCases(const TestGroup& testGroup, const GeneratorConfig& config) {
        string baseId = TestCaseIdCreator::createBaseId(config.slug(), testGroup.id());
        logger_->logMultipleTestCasesCombinationIntroduction(baseId);

        *config.multipleTestCasesCounter() = (int) testGroup.testCases().size();

        try {
            verify();
            combine(testGroup, config);
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

    void combine(const TestGroup& testGroup, const GeneratorConfig& config) {
        int testCaseCount = (int) testGroup.testCases().size();

        string baseId = TestCaseIdCreator::createBaseId(config.slug(), testGroup.id());
        string baseIn = config.outputDir() + "/" + baseId + ".in";
        string baseOut = config.outputDir() + "/" + baseId + ".out";
        bool generateOutput = config.generateOutput();

        ostringstream sout;

        sout << "echo " << testCaseCount << " > " << baseIn;

        if (generateOutput) {
            sout << " && touch " << baseOut;
        }

        os_->execute(ExecutionRequestBuilder().setCommand(sout.str()).build());

        for (int i = 1; i <= testCaseCount; i++) {
            string id = TestCaseIdCreator::create(config.slug(), testGroup.id(), i);
            string in = config.outputDir() + "/" + id + ".in";
            string out = config.outputDir() + "/" + id + ".out";

            ostringstream sout2;
            sout2 << "tail -n +2 " << in << " >> " << baseIn;

            if (generateOutput) {
                sout2 << " && ";
                if (i > 1 && config.multipleTestCasesOutputPrefix()) {
                    string outputPrefix = config.multipleTestCasesOutputPrefix().value();
                    // Replace the prefix for the first tc, with the correct prefix for this tc
                    string firstPrefix = StringUtils::interpolate(outputPrefix, 1);
                    string correctPrefix = StringUtils::interpolate(outputPrefix, i);
                    sout2 << "printf \"%b\" \"" << escapeForBash(correctPrefix) << "\" >> " << baseOut << " && ";
                    sout2 << "tail -c +" << (firstPrefix.size() + 1) << " " << out << " >> " << baseOut;
                } else {
                    sout2 << "cat " << out << " >> " << baseOut;
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

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
        string baseId = TestCaseIdCreator::createBaseId(config.slug(), testGroup.id());
        string baseFilename = config.outputDir() + "/" + baseId;
        os_->combineMultipleTestCases(baseFilename, (int) testGroup.testCases().size());
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

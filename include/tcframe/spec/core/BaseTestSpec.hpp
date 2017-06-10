#pragma once

#include <set>
#include <string>
#include <vector>

#include "GradingConfig.hpp"
#include "MultipleTestCasesConfig.hpp"
#include "SeedSetter.hpp"
#include "Spec.hpp"
#include "StyleConfig.hpp"
#include "tcframe/spec/random.hpp"
#include "tcframe/util.hpp"

using std::set;
using std::string;
using std::vector;

namespace tcframe {

template<typename TProblemSpec /* extends BaseProblemSpec */>
class BaseTestSpec : public TProblemSpec, protected TestSuiteBuilder {
private:
    vector<void(BaseTestSpec::*)()> sampleTestCases_ = {
            &BaseTestSpec::SampleTestCase1,
            &BaseTestSpec::SampleTestCase2,
            &BaseTestSpec::SampleTestCase3,
            &BaseTestSpec::SampleTestCase4,
            &BaseTestSpec::SampleTestCase5,
            &BaseTestSpec::SampleTestCase6,
            &BaseTestSpec::SampleTestCase7,
            &BaseTestSpec::SampleTestCase8,
            &BaseTestSpec::SampleTestCase9,
            &BaseTestSpec::SampleTestCase10,
            &BaseTestSpec::SampleTestCase11,
            &BaseTestSpec::SampleTestCase12,
            &BaseTestSpec::SampleTestCase13,
            &BaseTestSpec::SampleTestCase14,
            &BaseTestSpec::SampleTestCase15,
            &BaseTestSpec::SampleTestCase16,
            &BaseTestSpec::SampleTestCase17,
            &BaseTestSpec::SampleTestCase18,
            &BaseTestSpec::SampleTestCase19,
            &BaseTestSpec::SampleTestCase20,
            &BaseTestSpec::SampleTestCase21,
            &BaseTestSpec::SampleTestCase22,
            &BaseTestSpec::SampleTestCase23,
            &BaseTestSpec::SampleTestCase24,
            &BaseTestSpec::SampleTestCase25};

    vector<void(BaseTestSpec::*)()> testGroups_ = {
            &BaseTestSpec::TestGroup1,
            &BaseTestSpec::TestGroup2,
            &BaseTestSpec::TestGroup3,
            &BaseTestSpec::TestGroup4,
            &BaseTestSpec::TestGroup5,
            &BaseTestSpec::TestGroup6,
            &BaseTestSpec::TestGroup7,
            &BaseTestSpec::TestGroup8,
            &BaseTestSpec::TestGroup9,
            &BaseTestSpec::TestGroup10,
            &BaseTestSpec::TestGroup11,
            &BaseTestSpec::TestGroup12,
            &BaseTestSpec::TestGroup13,
            &BaseTestSpec::TestGroup14,
            &BaseTestSpec::TestGroup15,
            &BaseTestSpec::TestGroup16,
            &BaseTestSpec::TestGroup17,
            &BaseTestSpec::TestGroup18,
            &BaseTestSpec::TestGroup19,
            &BaseTestSpec::TestGroup20,
            &BaseTestSpec::TestGroup21,
            &BaseTestSpec::TestGroup22,
            &BaseTestSpec::TestGroup23,
            &BaseTestSpec::TestGroup24,
            &BaseTestSpec::TestGroup25};

public:
    virtual ~BaseTestSpec() {}

    TestSuite buildTestSuite(const string& slug, const set<int>& definedSubtaskIds) {
        TestSuiteBuilder::setSlug(slug);
        TestSuiteBuilder::setDefinedSubtaskIds(definedSubtaskIds);
        TestSuiteBuilder::setBeforeClosure([this] {
            BeforeTestCase();
        });
        TestSuiteBuilder::setAfterClosure([this] {
            AfterTestCase();
        });

        for (auto sampleTestCase : sampleTestCases_) {
            try {
                TestSuiteBuilder::newSampleTestCase();
                (this->*sampleTestCase)();
            } catch (NotImplementedException&) {
                break;
            }
        }

        try {
            TestCases();
        } catch (NotImplementedException&) {
            for (auto testGroup : testGroups_) {
                try {
                    TestSuiteBuilder::newTestGroup();
                    (this->*testGroup)();
                } catch (NotImplementedException&) {
                    break;
                }
            }
        }
        return TestSuiteBuilder::build();
    }

    virtual Spec buildSpec(const string& slug) {
        SeedSetter* seedSetter = new SeedSetter([=] (unsigned seed) {rnd.setSeed(seed);});
        IOFormat ioFormat = TProblemSpec::buildIOFormat();
        StyleConfig styleConfig = TProblemSpec::buildStyleConfig();
        GradingConfig gradingConfig = TProblemSpec::buildGradingConfig();
        MultipleTestCasesConfig multipleTestCasesConfig = TProblemSpec::buildMultipleTestCasesConfig();
        ConstraintSuite constraintSuite = TProblemSpec::buildConstraintSuite();
        TestSuite testSuite = buildTestSuite(slug, constraintSuite.getDefinedSubtaskIds());
        return Spec(
                seedSetter,
                ioFormat,
                styleConfig,
                gradingConfig,
                multipleTestCasesConfig,
                constraintSuite,
                testSuite);
    }

protected:
    Random rnd;

    virtual void BeforeTestCase() {}
    virtual void AfterTestCase() {}
    virtual void SampleTestCase1() {throw NotImplementedException();}
    virtual void SampleTestCase2() {throw NotImplementedException();}
    virtual void SampleTestCase3() {throw NotImplementedException();}
    virtual void SampleTestCase4() {throw NotImplementedException();}
    virtual void SampleTestCase5() {throw NotImplementedException();}
    virtual void SampleTestCase6() {throw NotImplementedException();}
    virtual void SampleTestCase7() {throw NotImplementedException();}
    virtual void SampleTestCase8() {throw NotImplementedException();}
    virtual void SampleTestCase9() {throw NotImplementedException();}
    virtual void SampleTestCase10() {throw NotImplementedException();}
    virtual void SampleTestCase11() {throw NotImplementedException();}
    virtual void SampleTestCase12() {throw NotImplementedException();}
    virtual void SampleTestCase13() {throw NotImplementedException();}
    virtual void SampleTestCase14() {throw NotImplementedException();}
    virtual void SampleTestCase15() {throw NotImplementedException();}
    virtual void SampleTestCase16() {throw NotImplementedException();}
    virtual void SampleTestCase17() {throw NotImplementedException();}
    virtual void SampleTestCase18() {throw NotImplementedException();}
    virtual void SampleTestCase19() {throw NotImplementedException();}
    virtual void SampleTestCase20() {throw NotImplementedException();}
    virtual void SampleTestCase21() {throw NotImplementedException();}
    virtual void SampleTestCase22() {throw NotImplementedException();}
    virtual void SampleTestCase23() {throw NotImplementedException();}
    virtual void SampleTestCase24() {throw NotImplementedException();}
    virtual void SampleTestCase25() {throw NotImplementedException();}
    virtual void TestCases() {throw NotImplementedException();}
    virtual void TestGroup1() {throw NotImplementedException();}
    virtual void TestGroup2() {throw NotImplementedException();}
    virtual void TestGroup3() {throw NotImplementedException();}
    virtual void TestGroup4() {throw NotImplementedException();}
    virtual void TestGroup5() {throw NotImplementedException();}
    virtual void TestGroup6() {throw NotImplementedException();}
    virtual void TestGroup7() {throw NotImplementedException();}
    virtual void TestGroup8() {throw NotImplementedException();}
    virtual void TestGroup9() {throw NotImplementedException();}
    virtual void TestGroup10() {throw NotImplementedException();}
    virtual void TestGroup11() {throw NotImplementedException();}
    virtual void TestGroup12() {throw NotImplementedException();}
    virtual void TestGroup13() {throw NotImplementedException();}
    virtual void TestGroup14() {throw NotImplementedException();}
    virtual void TestGroup15() {throw NotImplementedException();}
    virtual void TestGroup16() {throw NotImplementedException();}
    virtual void TestGroup17() {throw NotImplementedException();}
    virtual void TestGroup18() {throw NotImplementedException();}
    virtual void TestGroup19() {throw NotImplementedException();}
    virtual void TestGroup20() {throw NotImplementedException();}
    virtual void TestGroup21() {throw NotImplementedException();}
    virtual void TestGroup22() {throw NotImplementedException();}
    virtual void TestGroup23() {throw NotImplementedException();}
    virtual void TestGroup24() {throw NotImplementedException();}
    virtual void TestGroup25() {throw NotImplementedException();}
};

}

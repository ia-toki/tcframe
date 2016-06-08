#pragma once

#include <set>
#include <vector>

#include "CoreSpec.hpp"
#include "TestConfig.hpp"
#include "NotImplementedException.hpp"
#include "tcframe/testcase.hpp"

using std::vector;
using std::set;

namespace tcframe {

template<typename TProblemSpec /* extends BaseProblemSpec */>
class BaseTestSpec : public TProblemSpec, protected TestConfigBuilder, protected TestSuiteBuilder {
private:
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

    /* Hack: this is here because Config() is ambiguous with BaseProblemSpec's. */
    void applyProblemConfig() {
        TProblemSpec::Config();
    }

    TestSuite buildTestSuite() {
        SampleTestCases();

        try {
            TestCases();
            return TestSuiteBuilder::build();
        } catch (NotImplementedException&) {
            for (auto testGroup : testGroups_) {
                try {
                    TestSuiteBuilder::newTestGroup();
                    (this->*testGroup)();
                } catch (NotImplementedException&) {
                    break;
                }
            }
            return TestSuiteBuilder::buildWithoutLastTestGroup();
        }
    }

    TestConfig buildTestConfig() {
        Config();
        return TestConfigBuilder::build();
    }

    virtual CoreSpec buildCoreSpec() {
        return CoreSpec(
                CoreConfig(TProblemSpec::buildProblemConfig(), buildTestConfig()),
                TProblemSpec::buildIOFormat(),
                TProblemSpec::buildConstraintSuite(),
                buildTestSuite());
    }

protected:
    virtual void Config() {}
    virtual void SampleTestCases() {}
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

    void assignToSubtasks(const set<int>& subtaskIds) {
        TestSuiteBuilder::setConstraintGroupIds(subtaskIds);
    }
};

/* DEPRECATED. For backward compatibility with 0.x versions. */
template<typename TProblem>
class BaseGenerator : public BaseTestSpec<TProblem> {
public:
    virtual ~BaseGenerator() {}
};

}

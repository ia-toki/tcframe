#pragma once

#include <set>
#include <vector>

#include "CoreSpec.hpp"
#include "tcframe/spec/random.hpp"
#include "tcframe/spec/raw_testcase.hpp"
#include "tcframe/util.hpp"

using std::vector;
using std::set;

namespace tcframe {

template<typename TProblemSpec /* extends BaseProblemSpec */>
class BaseTestSpec : public TProblemSpec, protected RawTestSuiteBuilder {
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

    RawTestSuite buildRawTestSuite() {
        RawTestSuiteBuilder::setInputFinalizer([this] {
            InputFinalizer();
        });
        SampleTestCases();

        try {
            TestCases();
            return RawTestSuiteBuilder::build();
        } catch (NotImplementedException&) {
            for (auto testGroup : testGroups_) {
                try {
                    RawTestSuiteBuilder::newTestGroup();
                    (this->*testGroup)();
                } catch (NotImplementedException&) {
                    break;
                }
            }
            return RawTestSuiteBuilder::buildWithoutLastTestGroup();
        }
    }

    virtual CoreSpec buildCoreSpec() {
        return CoreSpec(
                TProblemSpec::buildProblemConfig(),
                TProblemSpec::buildIOFormat(),
                TProblemSpec::buildConstraintSuite(),
                buildRawTestSuite());
    }

protected:
    Random rnd;

    virtual void InputFinalizer() {}
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
        RawTestSuiteBuilder::setSubtaskIds(subtaskIds);
    }
};

}

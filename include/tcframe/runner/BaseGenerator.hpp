#pragma once

#include <set>
#include <vector>

#include "NotImplementedException.hpp"
#include "tcframe/config.hpp"
#include "tcframe/testcase.hpp"

using std::vector;
using std::set;

namespace tcframe {

template<typename TProblem /* extends BaseProblem */>
class BaseGenerator : public TProblem, protected GeneratorConfigBuilder, protected TestSuiteBuilder {
private:
    vector<void(BaseGenerator::*)()> testGroups_ = {
            &BaseGenerator::TestGroup1,
            &BaseGenerator::TestGroup2,
            &BaseGenerator::TestGroup3,
            &BaseGenerator::TestGroup4,
            &BaseGenerator::TestGroup5,
            &BaseGenerator::TestGroup6,
            &BaseGenerator::TestGroup7,
            &BaseGenerator::TestGroup8,
            &BaseGenerator::TestGroup9,
            &BaseGenerator::TestGroup10};

public:
    virtual ~BaseGenerator() {}

    void applyProblemConfig() {
        TProblem::Config();
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

    GeneratorConfig buildGeneratorConfig() {
        Config();
        return GeneratorConfigBuilder::build();
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

    void assignToSubtasks(const set<int>& subtaskIds) {
        TestSuiteBuilder::setConstraintGroupIds(subtaskIds);
    }
};

}

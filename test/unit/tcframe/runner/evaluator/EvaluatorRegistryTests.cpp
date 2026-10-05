#include "gmock/gmock.h"
#include "../../mock.hpp"

#include "../os/MockOperatingSystem.hpp"
#include "tcframe/runner/evaluator/EvaluatorRegistry.hpp"

using ::testing::Eq;
using ::testing::Test;

namespace tcframe {

class EvaluatorRegistryTests : public Test {
protected:
    MOCK(OperatingSystem) os;
    EvaluatorHelperRegistry helperRegistry;
    EvaluatorRegistry registry{&helperRegistry};
};

TEST_F(EvaluatorRegistryTests, Get_Functional) {
    Evaluator* evaluator = registry.get("functional", &os, {{"manager", "./mgr"}});

    EXPECT_NE(dynamic_cast<FunctionalEvaluator*>(evaluator), nullptr);
}

TEST_F(EvaluatorRegistryTests, GetConfig_Functional_TcOutputOptional) {
    EXPECT_THAT(registry.getConfig("functional").testCaseOutputType(), Eq(TestCaseOutputType::OPTIONAL));
}

TEST_F(EvaluatorRegistryTests, Get_OutputOnly) {
    Evaluator* evaluator = registry.get("output_only", &os, {});

    EXPECT_NE(dynamic_cast<OutputOnlyEvaluator*>(evaluator), nullptr);
}

TEST_F(EvaluatorRegistryTests, Get_OutputOnly_CustomScorer) {
    Evaluator* evaluator = registry.get("output_only", &os, {{"scorer", "./myscorer"}});

    EXPECT_NE(dynamic_cast<OutputOnlyEvaluator*>(evaluator), nullptr);
}

TEST_F(EvaluatorRegistryTests, GetConfig_OutputOnly_TestCaseOutputOptional) {
    EvaluatorConfig config = registry.getConfig("output_only");

    EXPECT_THAT(config.testCaseOutputType(), Eq(TestCaseOutputType::OPTIONAL));
}

}

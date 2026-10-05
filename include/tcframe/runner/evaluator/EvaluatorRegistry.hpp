#pragma once

#include <map>
#include <string>

#include "BatchEvaluator.hpp"
#include "EvaluatorConfig.hpp"
#include "EvaluatorHelperRegistry.hpp"
#include "FunctionalEvaluator.hpp"
#include "InteractiveEvaluator.hpp"
#include "OutputOnlyEvaluator.hpp"
#include "communicator.hpp"
#include "scorer.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"

using std::map;
using std::string;

namespace tcframe {

class EvaluatorRegistry {
private:
    EvaluatorHelperRegistry* helperRegistry_;

public:
    virtual ~EvaluatorRegistry() = default;

    explicit EvaluatorRegistry(EvaluatorHelperRegistry* helperRegistry)
            : helperRegistry_(helperRegistry) {}

    virtual Evaluator* get(
            const string& slug,
            OperatingSystem* os,
            const map<string, string>& helperCommands,
            const string& scorerArgs = "") {
        if (slug == "batch") {
            return getBatch(os, helperCommands, scorerArgs);
        }
        if (slug == "interactive") {
            return getInteractive(os, helperCommands);
        }
        if (slug == "output_only") {
            return getOutputOnly(os, helperCommands, scorerArgs);
        }
        if (slug == "functional") {
            return getFunctional(os, helperCommands, scorerArgs);
        }
        return nullptr;
    }

    virtual EvaluatorConfig getConfig(const string& slug) {
        if (slug == "batch") {
            return getBatchConfig();
        }
        if (slug == "interactive") {
            return getInteractiveConfig();
        }
        if (slug == "output_only") {
            return getOutputOnlyConfig();
        }
        if (slug == "functional") {
            return getFunctionalConfig();
        }
        return {};
    }

private:
    Evaluator* getBatch(OperatingSystem* os, const map<string, string>& helperCommands, const string& scorerArgs) {
        Scorer* scorer = helperRegistry_->getScorer(os, getHelperCommand(helperCommands, "scorer"), scorerArgs);

        return new BatchEvaluator(os, new TestCaseVerdictParser(), scorer);
    }

    EvaluatorConfig getBatchConfig() {
        return EvaluatorConfigBuilder()
                .setTestCaseOutputType(TestCaseOutputType::OPTIONAL)
                .build();
    }

    Evaluator* getInteractive(OperatingSystem* os, const map<string, string>& helperCommands) {
        string communicatorCommand = getHelperCommand(helperCommands, "communicator").value();
        Communicator* communicator = helperRegistry_->getCommunicator(os, communicatorCommand);

        return new InteractiveEvaluator(communicator);
    }

    EvaluatorConfig getInteractiveConfig() {
        return EvaluatorConfigBuilder()
                .setTestCaseOutputType(TestCaseOutputType::NOT_REQUIRED)
                .build();
    }

    Evaluator* getOutputOnly(OperatingSystem* os, const map<string, string>& helperCommands, const string& scorerArgs) {
        Scorer* scorer = helperRegistry_->getScorer(os, getHelperCommand(helperCommands, "scorer"), scorerArgs);

        return new OutputOnlyEvaluator(os, new TestCaseVerdictParser(), scorer);
    }

    EvaluatorConfig getOutputOnlyConfig() {
        return EvaluatorConfigBuilder()
                .setTestCaseOutputType(TestCaseOutputType::OPTIONAL)
                .build();
    }

    Evaluator* getFunctional(OperatingSystem* os, const map<string, string>& helperCommands, const string& scorerArgs) {
        Scorer* scorer = helperRegistry_->getScorer(os, getHelperCommand(helperCommands, "scorer"), scorerArgs);
        string managerDir = getHelperCommand(helperCommands, "manager").value_or("./manager");
        string evaluatorDir = getHelperCommand(helperCommands, "evaluator").value_or("");
        string solutionFamily = getHelperCommand(helperCommands, "solution-family").value_or("cpp");

        return new FunctionalEvaluator(os, new TestCaseVerdictParser(), scorer, managerDir, evaluatorDir, solutionFamily);
    }

    EvaluatorConfig getFunctionalConfig() {
        return EvaluatorConfigBuilder()
                .setTestCaseOutputType(TestCaseOutputType::OPTIONAL)
                .build();
    }

    static optional<string> getHelperCommand(const map<string, string>& helperCommands, const string& key) {
        if (helperCommands.count(key)) {
            return optional<string>(helperCommands.at(key));
        }
        return {};
    }
};

}

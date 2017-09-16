#pragma once

#include <map>
#include <string>

#include "BatchEvaluator.hpp"
#include "EvaluatorConfig.hpp"
#include "EvaluatorHelperRegistry.hpp"
#include "InteractiveEvaluator.hpp"
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

    virtual Evaluator* get(const string& slug, OperatingSystem* os, const map<string, string>& helperCommands) {
        if (slug == "batch") {
            return getBatch(os, helperCommands);
        }
        if (slug == "interactive") {
            return getInteractive(os, helperCommands);
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
        return {};
    }

private:
    Evaluator* getBatch(OperatingSystem* os, const map<string, string>& helperCommands) {
        Scorer* scorer = helperRegistry_->getScorer(os, getHelperCommand(helperCommands, "scorer"));

        return new BatchEvaluator(os, new VerdictCreator(), scorer);
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

    static optional<string> getHelperCommand(const map<string, string>& helperCommands, const string& key) {
        if (helperCommands.count(key)) {
            return optional<string>(helperCommands.at(key));
        }
        return {};
    }
};

}

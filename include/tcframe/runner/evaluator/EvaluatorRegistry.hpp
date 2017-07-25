#pragma once

#include <map>
#include <string>

#include "BatchEvaluator.hpp"
#include "EvaluatorConfig.hpp"
#include "EvaluatorHelperRegistry.hpp"
#include "InteractiveEvaluator.hpp"
#include "communicator.hpp"
#include "scorer.hpp"
#include "tcframe/os.hpp"
#include "tcframe/spec.hpp"
#include "tcframe/util.hpp"
#include "tcframe/verdict.hpp"

using std::map;
using std::string;

namespace tcframe {

class EvaluatorRegistry {
private:
    EvaluatorHelperRegistry* helperRegistry_;

public:
    virtual ~EvaluatorRegistry() {}

    EvaluatorRegistry(EvaluatorHelperRegistry* helperRegistry)
            : helperRegistry_(helperRegistry) {}

    virtual Evaluator* get(EvaluationStyle style, OperatingSystem* os, const map<string, string>& helperCommands) {
        switch (style) {
            case EvaluationStyle::BATCH:
                return getBatch(os, helperCommands);
            case EvaluationStyle::INTERACTIVE:
                return getInteractive(os, helperCommands);
        }
    }

    virtual EvaluatorConfig getConfig(EvaluationStyle style) {
        switch (style) {
            case EvaluationStyle::BATCH:
                return getBatchConfig();
            case EvaluationStyle::INTERACTIVE:
                return getInteractiveConfig();
        }
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
        return optional<string>();
    }
};

}

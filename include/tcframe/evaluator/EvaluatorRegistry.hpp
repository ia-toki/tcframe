#pragma once

#include <map>
#include <string>

#include "BatchEvaluator.hpp"
#include "EvaluatorHelperRegistry.hpp"
#include "scorer.hpp"
#include "tcframe/os.hpp"
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

    virtual Evaluator* get(OperatingSystem* os, const map<string, string>& helperCommands) {
        return getBatch(os, helperCommands);
    }

private:
    Evaluator* getBatch(OperatingSystem* os, const map<string, string>& helperCommands) {
        Scorer* scorer = helperRegistry_->getScorer(os, getHelperCommand(helperCommands, "scorer"));

        return new BatchEvaluator(os, new VerdictCreator(), scorer);
    }

    static optional<string> getHelperCommand(const map<string, string>& helperCommands, const string& key) {
        if (helperCommands.count(key)) {
            return optional<string>(helperCommands.at(key));
        }
        return optional<string>();
    }
};

}

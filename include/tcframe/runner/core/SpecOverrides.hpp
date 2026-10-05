#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Args.hpp"
#include "tcframe/spec/core/SpecYaml.hpp"
#include "tcframe/util.hpp"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

// Applies inputs discovered by the Python CLI (packages, registry, languages)
// onto the compiled spec, so the single C++ writer emits the final spec.yml (SPEC.md T0.4).
// Formats:
//   --solution-keys=encoder,decoder
//   --helper=<name>=<slug>            (repeatable; e.g. scorer=compare)
//   --helper-args=<name>=<args>       (repeatable; e.g. scorer=float_tolerance 9)
//   --aggregator=<N>=<slug>           (repeatable; N is 1-based subtask index)
//   --aggregator-args=<N>=<args>      (repeatable)
class SpecOverrides {
public:
    SpecOverrides() = delete;

    static void apply(const Args& args, SpecYaml& spec) {
        if (args.solutionKeys()) {
            spec.evaluator.solution_keys = parseSolutionKeys(args.solutionKeys().value());
        }

        for (const string& item : args.helpers()) {
            pair<string, string> kv = parseKeyValue("--helper", item);
            spec.helpers[kv.first].slug = kv.second;
        }

        for (const string& item : args.helperArgs()) {
            pair<string, string> kv = parseKeyValue("--helper-args", item);
            spec.helpers[kv.first].additional_args = kv.second;
        }

        for (const string& item : args.aggregators()) {
            pair<string, string> kv = parseKeyValue("--aggregator", item);
            subtaskAt(spec, kv.first, "--aggregator").aggregator.slug = kv.second;
        }

        for (const string& item : args.aggregatorArgs()) {
            pair<string, string> kv = parseKeyValue("--aggregator-args", item);
            subtaskAt(spec, kv.first, "--aggregator-args").aggregator.args = kv.second;
        }
    }

private:
    static vector<string> parseSolutionKeys(const string& value) {
        vector<string> keys = StringUtils::split(value, ',');
        if (keys.empty()) {
            throw runtime_error("tcframe: --solution-keys must not be empty");
        }
        for (const string& key : keys) {
            if (key.empty()) {
                throw runtime_error("tcframe: --solution-keys has an empty key in '" + value + "'");
            }
        }
        return keys;
    }

    static pair<string, string> parseKeyValue(const string& option, const string& item) {
        size_t eq = item.find('=');
        if (eq == string::npos || eq == 0) {
            throw runtime_error("tcframe: " + option + " expects key=value, got '" + item + "'");
        }
        return make_pair(item.substr(0, eq), item.substr(eq + 1));
    }

    static SubtaskYaml& subtaskAt(SpecYaml& spec, const string& index, const string& option) {
        optional<int> n = StringUtils::toNumber<int>(index);
        if (!n || n.value() < 1 || n.value() > int(spec.subtasks.size())) {
            throw runtime_error("tcframe: " + option + " subtask index '" + index + "' is out of range");
        }
        return spec.subtasks[n.value() - 1];
    }
};

}

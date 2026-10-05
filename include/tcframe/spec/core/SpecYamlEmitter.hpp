#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>

#include "tcframe/spec/core/SpecYaml.hpp"

using std::ostream;
using std::ostringstream;
using std::set;
using std::string;

namespace tcframe {

// Hand-rolled YAML writer for SpecYaml (no deps). Fixed schema, see SPEC.md §4.3.
class SpecYamlEmitter {
public:
    SpecYamlEmitter() = delete;

    static string emit(const SpecYaml& spec) {
        ostringstream out;
        emit(out, spec);
        return out.str();
    }

    static void emit(ostream& out, const SpecYaml& spec) {
        out << "slug: " << scalar(spec.slug) << "\n";

        if (spec.subtasks.empty()) {
            out << "subtasks: []\n";
        } else {
            out << "subtasks:\n";
            for (const SubtaskYaml& subtask : spec.subtasks) {
                out << "  - points: " << number(subtask.points) << "\n";
                if (!subtask.aggregator.slug.empty() || !subtask.aggregator.args.empty()) {
                    out << "    aggregator:\n";
                    if (!subtask.aggregator.slug.empty()) {
                        out << "      slug: " << scalar(subtask.aggregator.slug) << "\n";
                    }
                    if (!subtask.aggregator.args.empty()) {
                        out << "      args: " << scalar(subtask.aggregator.args) << "\n";
                    }
                }
            }
        }

        // RFC: evaluator.slug is left out for a custom evaluator.
        out << "evaluator:\n";
        if (!spec.evaluator.slug.empty()) {
            out << "  slug: " << scalar(spec.evaluator.slug) << "\n";
        }
        out << "  solution_keys: [";
        for (size_t i = 0; i < spec.evaluator.solution_keys.size(); i++) {
            if (i > 0) {
                out << ", ";
            }
            out << scalar(spec.evaluator.solution_keys[i]);
        }
        out << "]\n";
        out << "  tc_output_present: " << boolean(spec.evaluator.tc_output_present) << "\n";

        if (spec.helpers.empty()) {
            out << "helpers: {}\n";
        } else {
            out << "helpers:\n";
            for (const auto& helper : spec.helpers) {
                const HelperYaml& value = helper.second;
                if (value.slug.empty() && value.additional_args.empty()) {
                    out << "  " << scalar(helper.first) << ": {}\n";
                    continue;
                }
                out << "  " << scalar(helper.first) << ":\n";
                if (!value.slug.empty()) {
                    out << "    slug: " << scalar(value.slug) << "\n";
                }
                if (!value.additional_args.empty()) {
                    out << "    additional_args: " << scalar(value.additional_args) << "\n";
                }
            }
        }

        // Units per SPEC.md §4.3: time in ms, memory in kb.
        out << "limits:\n";
        out << "  time: " << spec.limits.time_s * 1000 << "\n";
        out << "  memory: " << spec.limits.memory_mb * 1024 << "\n";
    }

private:
    static string boolean(bool value) {
        return value ? "true" : "false";
    }

    static string number(double value) {
        ostringstream out;
        out << value;
        return out.str();
    }

    // Plain for identifiers and integers; otherwise double-quoted with escapes.
    static string scalar(const string& value) {
        if (isPlainSafe(value)) {
            return value;
        }
        string quoted = "\"";
        for (char c : value) {
            if (c == '"' || c == '\\') {
                quoted += '\\';
            }
            quoted += c;
        }
        quoted += "\"";
        return quoted;
    }

    static bool isPlainSafe(const string& value) {
        static const set<string> RESERVED = {"true", "false", "yes", "no", "on", "off", "null", "~"};
        if (value.empty() || RESERVED.count(value) > 0) {
            return false;
        }

        bool isInteger = true;
        size_t start = value[0] == '-' ? 1 : 0;
        if (start == value.size()) {
            return false;
        }
        for (size_t i = start; i < value.size(); i++) {
            if (value[i] < '0' || value[i] > '9') {
                isInteger = false;
                break;
            }
        }
        if (isInteger) {
            return true;
        }

        if (!((value[0] >= 'a' && value[0] <= 'z') || (value[0] >= 'A' && value[0] <= 'Z') || value[0] == '_')) {
            return false;
        }
        for (char c : value) {
            bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')
                    || c == '_' || c == '-' || c == '.';
            if (!ok) {
                return false;
            }
        }
        return true;
    }
};

}

#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "BatchEvaluator.hpp"
#include "EvaluationOptions.hpp"
#include "EvaluationResult.hpp"
#include "EvaluatorScripts.hpp"
#include "GenerationResult.hpp"
#include "scorer.hpp"
#include "tcframe/runner/os.hpp"
#include "tcframe/runner/verdict.hpp"

using std::runtime_error;
using std::string;
using std::vector;

namespace tcframe {

// Functional (RFC 2.0, SPEC.md T6.2): contestants submit several source files, one per solution key.
// They are linked with the grader from the manager helper directory (manager/cpp/grader.cpp, with
// per-key headers next to it), and the resulting program is run like a batch solution.
//
// The solution is built once per evaluator instance. Build and run go through build_[family] and
// run_[family] scripts of the evaluator directory when they exist (SPEC.md T6.3); otherwise the
// built-in C++ build is used, and any other family needs its build script.
class FunctionalEvaluator : public BatchEvaluator {
public:
    static constexpr const char* BINARY_FILENAME = "__tcframe_functional";

private:
    string managerDir_;
    string solutionFamily_;
    EvaluatorScripts scripts_;
    string binary_;

public:
    virtual ~FunctionalEvaluator() = default;

    FunctionalEvaluator(
            OperatingSystem* os,
            TestCaseVerdictParser* testCaseVerdictParser,
            Scorer* scorer,
            const string& managerDir,
            const string& evaluatorDir = "",
            const string& solutionFamily = "cpp")
            : BatchEvaluator(os, testCaseVerdictParser, scorer)
            , managerDir_(managerDir)
            , solutionFamily_(solutionFamily)
            , scripts_(os, evaluatorDir) {}

    GenerationResult generate(
            const string& inputFilename,
            const string& outputFilename,
            const EvaluationOptions& options) {

        if (binary_.empty()) {
            binary_ = build(options);
        }

        string runCommand = scripts_.hasRun(solutionFamily_)
                ? scripts_.runCommand(solutionFamily_, inputFilename, {managerDir_})
                : "./" + string(BINARY_FILENAME);
        EvaluationOptions runOptions = EvaluationOptionsBuilder(options)
                .setSolutionCommand(runCommand)
                .build();
        return BatchEvaluator::generate(inputFilename, outputFilename, runOptions);
    }

private:
    string build(const EvaluationOptions& options) {
        const SolutionMap& solutions = options.solutions();
        if (solutions.keys().empty()) {
            throw runtime_error("tcframe: functional evaluation needs at least one solution key");
        }

        vector<string> files;
        for (const string& key : solutions.keys()) {
            files.push_back(solutions.get(key));
        }

        string command;
        if (scripts_.hasBuild(solutionFamily_)) {
            command = scripts_.buildCommand(solutionFamily_, files, {managerDir_});
        } else if (solutionFamily_ == "cpp") {
            command = "g++ -std=c++17 -O2 -I " + managerDir_ + "/cpp -o " + BINARY_FILENAME + " " +
                      managerDir_ + "/cpp/grader.cpp";
            for (const string& file : files) {
                command += " " + file;
            }
        } else {
            throw runtime_error("tcframe: no build_" + solutionFamily_ + " script for functional solutions in the evaluator directory");
        }

        ExecutionResult executionResult = os_->execute(ExecutionRequestBuilder().setCommand(command).build());
        if (!executionResult.isSuccessful()) {
            throw runtime_error("tcframe: failed to build functional solution:\n" + executionResult.standardError());
        }
        return BINARY_FILENAME;
    }
};

}

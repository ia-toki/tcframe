#include "tcframe/aggregator.hpp"
#include "tcframe/evaluator.hpp"
#include "tcframe/generator.hpp"
#include "tcframe/grader.hpp"
#include "tcframe/os.hpp"
#include "tcframe/runner.hpp"
#include "tcframe/spec.hpp"

#include __TCFRAME_SPEC_FILE__

using tcframe::Runner;

int main(int argc, char* argv[]) {
    Runner<ProblemSpec> runner(
            __TCFRAME_SPEC_FILE__,
            new TestSpec(),
            new SimpleLoggerEngine(),
            new UnixOperatingSystem(),
            new RunnerLoggerFactory(),
            new GeneratorFactory(),
            new GraderFactory(),
            new EvaluatorRegistry(new EvaluatorHelperRegistry()),
            new AggregatorRegistry());

    return runner.run(argc, argv);
}

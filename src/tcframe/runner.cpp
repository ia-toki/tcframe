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
    auto testSpec = new TestSpec();
    auto loggerEngine = new SimpleLoggerEngine();
    auto os = new UnixOperatingSystem();
    auto runnerLoggerFactory = new RunnerLoggerFactory();
    auto evaluatorFactory = new EvaluatorFactory();
    auto aggregatorFactory = new AggregatorFactory();
    auto generatorFactory = new GeneratorFactory();
    auto graderFactory = new GraderFactory();

    Runner<ProblemSpec> runner(
            __TCFRAME_SPEC_FILE__,
            testSpec,
            loggerEngine,
            os,
            runnerLoggerFactory,
            evaluatorFactory,
            aggregatorFactory,
            generatorFactory,
            graderFactory);

    return runner.run(argc, argv);
}

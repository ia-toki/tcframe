# Registry

The registry holds the defaults that every problem uses unless its package overrides them: the default scorer, the default evaluators, the subtask aggregators, and the functional build and run scripts. It lives in `registry/` in the TCFrame checkout.

## Layout

```
registry/
  aggregators/{min,sum,threshold}/run      subtask aggregators
  helpers/scorer/compare/run               default scorer
  helpers/scorer/defaults.yml              names the default scorer (slug: compare)
  evaluators/{batch,interactive,output_only,functional}/evaluator.yml
  evaluators/functional/{build,run}_{cpp,pascal}
```

The registry is searched in this order: `$TCFRAME_HOME/registry`, then `~/.tcframe/registry`.

## Overriding a default

A problem package can override some registry entries:

- An `aggregator_<subtasks>` program in the package replaces the registry aggregator for those subtasks. A plain `aggregator` applies to all subtasks.
- A `manager/` folder supplies the grader for functional problems. See [Multi-file solutions](../solutions/multi-file).

To change the registry itself, edit the files in `registry/`. Keep the interface the same, because every problem depends on it.

## The default scorer

`helpers/scorer/compare/run` is the default scorer. It is a POSIX shell script that uses only `sh` and `awk`. Its arguments are the input, the expected output, and the contestant output. It prints `AC` or `WA`, and on `WA` the first 10 lines of the difference go to stderr.

## Evaluator definitions

Each `evaluator.yml` declares what a problem must provide:

| Key | Meaning |
| --- | --- |
| `custom_solution_keys` | Whether the evaluator takes solution keys other than `source` |
| `tc_output` | `required`, `not_required`, or `optional` |
| `helpers` | Programs or folders the evaluator needs, with `slug`, `type`, and `optional` |

See [Output-only](../grading/output-only) and [Interactive](../grading/interactive) for the evaluators that use these settings.

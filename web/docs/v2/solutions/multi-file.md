# Multi-file Solutions

Some problems ask contestants to submit several functions, for example an `encoder` and a `decoder` that must work together. Each solution is then a folder with one source file per key.

## Spec

Name the keys in `StyleConfig` and use the functional evaluator:

```cpp
void StyleConfig() {
    FunctionalEvaluator({"encoder", "decoder"});
    NoOutput();   // optional, if no expected outputs are needed
}
```

## Package layout

```
mypkg/
  spec.cpp
  manager/
    cpp/
      grader.cpp        main program that calls the contestant's functions
      encoder.h         declares encode(...)
      decoder.h         declares decode(...)
  solutions/
    ref/fushar/
      encoder.cpp
      decoder.cpp
    wa/bad/
      encoder.cpp
      decoder.cpp
```

- Each folder under `solutions/` contains one file per key. The file names must match the keys (`encoder`, `decoder`), with the extension of the language, for example `.cpp`.
- `manager/<language>/` holds the grader and the header files. The shipped families are `cpp` and `pascal`.

## Checking

```sh
tcframe test
```

`test` builds each folder with the functional build and run scripts for its language, then grades it like any other solution.

A missing file shows up as an error, for example:

```
ERROR  wa/bad  '.../solutions/wa/bad' has no file for solution key 'encoder' (expected encoder.<ext>)
```

## Notes

- The shipped build and run scripts are in `registry/evaluators/functional/` (see [Registry](../extending/registry)).
- The contestant's code never runs `main`. The grader's `main` calls their functions.

# Compiling Solutions

You never compile solutions by hand. `tcframe` picks a language from the file extension and builds each file for you.

## Languages

Languages are defined in `languages/*.yml`. The shipped ones are:

| File | Extensions | Build | Run |
| --- | --- | --- | --- |
| `cpp17.yml` | `cc`, `cpp`, `c++` | `g++ -std=c++17 -O2` | the binary |
| `pascal.yml` | `pas` | `fpc -O2` | the binary |

If a file's extension isn't listed, `tcframe` stops with an error that names the extension. See [Languages](../extending/languages) to add one.

## Where the builds go

Everything is built under the build directory (`./build` by default):

- `build/solutions/` holds each compiled solution.
- `build/helpers/` holds compiled scorers and communicators.

Your package stays clean. Delete `build/` at any time and the next command rebuilds everything.

## Checking a build by hand

If a solution fails to compile, reproduce the build with the same flags:

```sh
g++ -std=c++17 -O2 -o /tmp/check solutions/ref/solution.cpp
```

## Compile errors

A solution that doesn't compile is reported as an error for that solution, with the compiler output. Fix the source and run the command again.

`make` and `package` also stop when the reference solution doesn't compile, because the expected outputs can't be generated without it.

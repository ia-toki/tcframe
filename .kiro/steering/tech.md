# Tech

The repository is a monorepo containing the C++ framework, the Python CLI that
drives it, and a docs site.

## C++ framework (the core)

- **Language:** C++17.
- **Shape:** header-only library under `include/tcframe/`. There is no compiled
  library artifact; a user's `spec.cpp` includes the headers and is compiled
  into a standalone `runner` executable.
- **Compiler:** GCC with C++17 support. The `scripts/tcframe` wrapper compiles a
  spec with:
  `g++ -std=c++17 -D__TCFRAME_SPEC_FILE__="..." -I "$TCFRAME_HOME/include" ... src/tcframe/runner.cpp`.
- **Environment:** requires the `TCFRAME_HOME` environment variable to point at
  the install root. Optional `TCFRAME_CXX_FLAGS` are passed through to the
  compiler.
- **CLI:** `scripts/tcframe` is a thin shim that runs the Python CLI (see below).
  Its `TCFRAME_HOME` defaults to the checkout when unset; an already-exported
  value wins.
- **Platforms:** Linux and macOS only.

### Build and test (C++)

CMake drives the test build (`CMakeLists.txt`):

```bash
cmake . && cmake --build .   # configure + build
./test_unit                  # unit tests
./test_integration           # integration tests
./test_ete                   # end-to-end tests
```

- **Test framework:** GoogleTest + GoogleMock (fetched via CMake
  `ExternalProject`, release-1.8.0).
- **Coverage:** compiled with `--coverage` (gcov), reported to Codecov.
- Tests live under `test/{unit,integration,ete}/` and mirror the `include/`
  layout. Mocks are `Mock*.hpp` files alongside the tests.

## Python CLI (cli/)

- **Language:** Python >= 3.10, stdlib only (**zero runtime dependencies**).
- **Packaging:** `cli/pyproject.toml` with the `hatchling` build backend;
  distribution `tcframe-cli`, import `tcframe_cli`, console script `tcframe`.
- **Run model:** `scripts/tcframe <cmd>` → `python3 -m tcframe_cli <cmd>` with
  `cli/` on `PYTHONPATH`. `TCFRAME_PYTHON` overrides the interpreter. The CLI
  compiles `spec.cpp` with the C++ toolchain, runs the built `spec` binary for
  `spec.yml` and `tc/`, and compiles/grades solutions. Specs are never Python.

### Test (Python CLI)

- Tests live under `cli/tests/`: `cd cli && python -m pytest`. Real-`g++`
  integration tests cover compile, `make`, `package`, and `validate`.

## Docs site (web/)

- A static documentation site (Node/Yarn based) built and deployed from
  `web/`, published to https://tcframe.toki.id on pushes to `master`.

## CI

- GitHub Actions (`.github/workflows/main.yml`), `ci` workflow on
  `ubuntu-latest`: builds the CMake project, runs unit/integration/ete tests,
  uploads coverage. A separate `deploy-web` job builds and rsyncs the docs site
  on `master` (restricted to the `ia-toki` repository owner).

## Common commands

| Task | Command |
| --- | --- |
| Build a C++ spec | `TCFRAME_HOME=... scripts/tcframe build` (in the problem dir) |
| Generate spec.yml + tc/ | `scripts/tcframe make --solution=solutions/ref/solution.cpp` (in the problem dir) |
| Check solutions vs expected verdicts | `scripts/tcframe test` (in the problem dir) |
| Make source + distribution zips | `scripts/tcframe package --solution=solutions/ref/solution.cpp` |
| Configure + build tests | `cmake . && cmake --build .` |
| Run C++ test suites | `./test_unit` / `./test_integration` / `./test_ete` |
| Run Python CLI tests | `cd cli && python -m pytest` |
| Try an example | `cd examples/<name> && ../../scripts/tcframe make --solution=solutions/ref/solution.cpp` |

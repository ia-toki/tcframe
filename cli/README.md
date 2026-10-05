# tcframe CLI

Python orchestration CLI for the tcframe 2.0 toolchain. Drives the C++ `spec.cpp`
program and its `runner`; zero runtime dependencies (stdlib only).

Status: skeleton (T1.1). Subcommands (`new`, `build`, `make`, `test`, `grade`,
`package`) arrive in later Phase 1+ tasks.

## Development

```bash
cd cli
uv venv && uv pip install -e ".[dev]"
python -m pytest
```

"""
`tcframe build`: compile `spec.cpp` into a `spec` binary inside the build
directory, write its `spec.yml`, then run test case generation through it.

The compile line matches the 1.x `scripts/tcframe build` wrapper, with the
output moved into the build directory.
"""

import os
import re
import shlex
import shutil
import subprocess
from pathlib import Path
from typing import Mapping, Optional, Sequence

from tcframe_cli.compiler import CompiledProgram, CompileError, absolute_command, compile_program, language_for
from tcframe_cli.languages import available_languages
from tcframe_cli.multifile import MultifileError, multifile_args
from tcframe_cli.paths import ENV_TCFRAME_HOME, tcframe_home
from tcframe_cli.process import check_command
from tcframe_cli.scaffold import SPEC_FILE

DEFAULT_BUILD_DIR = 'build'
DEFAULT_SOLUTION = './solution'
SPEC_BINARY = 'spec'
SPEC_YML = 'spec.yml'
TC_DIR = 'tc'
COMPILER = 'g++'
ENV_CXX_FLAGS = 'TCFRAME_CXX_FLAGS'
RUNNER_SOURCE = Path('src') / 'tcframe' / 'runner.cpp'
INCLUDE_DIR = 'include'

# Build-dir subdirectory and program name for a source reference solution (SPEC.md T7.3).
REFERENCE_BUILD_DIR = 'solutions'
REFERENCE_NAME = 'ref'

_TC_OUTPUT_PRESENT = re.compile(r'^\s*tc_output_present:\s*(true|false)\s*$', re.MULTILINE)


class BuildError(Exception):
    """Raised when the spec cannot be compiled or generated; the message is user-facing."""


def _require_home(env: Mapping[str, str]) -> Path:
    home = tcframe_home(env)
    if home is None:
        raise BuildError(f'{ENV_TCFRAME_HOME} is not set; point it at the tcframe install root')
    for rel in (INCLUDE_DIR, RUNNER_SOURCE):
        if not (home / rel).exists():
            raise BuildError(f"'{home / rel}' not found; {ENV_TCFRAME_HOME} must point at the tcframe install root")
    return home


def compile_command(spec: Path, binary: Path, home: Path, extra_flags: list[str]) -> list[str]:
    """argv for compiling the spec and the shared runner TU into `binary`."""
    return [
        COMPILER, '-std=c++17',
        f'-D__TCFRAME_SPEC_FILE__="{spec}"',
        '-I', str(home / INCLUDE_DIR),
        *extra_flags,
        '-o', str(binary),
        str(home / RUNNER_SOURCE),
    ]


def compile_spec(
    package_dir: Path,
    build_dir: Path,
    env: Optional[Mapping[str, str]] = None,
) -> Path:
    """Compile `package_dir/spec.cpp` into `build_dir/spec` and return the binary path."""
    if env is None:
        env = os.environ

    spec = package_dir / SPEC_FILE
    if not spec.is_file():
        raise BuildError(f"spec file '{spec}' does not exist")

    home = _require_home(env)
    extra_flags = shlex.split(env.get(ENV_CXX_FLAGS, ''))

    if shutil.which(COMPILER) is None:
        raise BuildError(f"'{COMPILER}' not found on PATH")

    build_dir.mkdir(parents=True, exist_ok=True)
    binary = build_dir / SPEC_BINARY
    argv = compile_command(spec.resolve(), binary.resolve(), home, extra_flags)

    result = subprocess.run(argv, capture_output=True, text=True)
    if result.returncode != 0:
        raise BuildError(f'compiling {spec} failed:\n{result.stderr.rstrip()}')
    return binary


def anchor_program(package_dir: Path, command: str) -> str:
    """Make a relative program path in `command` absolute against `package_dir`."""
    parts = shlex.split(command)
    if not parts or os.path.isabs(parts[0]) or '/' not in parts[0]:
        return command
    parts[0] = str(package_dir / parts[0])
    return shlex.join(parts)


HELPER_BUILD_DIR = 'helpers'


def resolve_helper(
    package_dir: Path,
    build_dir: Path,
    command: Optional[str],
    name: str,
    env: Optional[Mapping[str, str]] = None,
) -> Optional[str]:
    """
    Absolute command for a scorer or communicator. A source file (known extension) or
    a program directory is compiled into build_dir/helpers/<name>/ first, since helpers
    are built automatically (RFC). Anything else is anchored to package_dir as before.
    Extra words after the program path are kept as arguments.
    """
    if command is None:
        return None
    parts = shlex.split(command)
    if not parts:
        return anchor_program(package_dir, command)
    source = Path(parts[0]) if os.path.isabs(parts[0]) else package_dir / parts[0]
    if not source.is_file() and not source.is_dir():
        return anchor_program(package_dir, command)
    languages = available_languages(package_dir, env)
    if source.is_file() and language_for(source, languages) is None:
        return anchor_program(package_dir, command)
    try:
        compiled = compile_program(source, build_dir / HELPER_BUILD_DIR, languages, name=name)
    except CompileError as exc:
        raise BuildError(f'{name} failed to compile: {exc}')
    return absolute_command(CompiledProgram(compiled.name, compiled.workdir, compiled.argv + parts[1:]))


def emit_spec_yml(binary: Path, package_dir: Path, out_path: Path) -> None:
    """Run the compiled spec's `spec` command, writing its SpecYaml to `out_path`."""
    argv = [str(binary.resolve()), 'spec', f'--spec-file={out_path.resolve()}']
    result = subprocess.run(argv, cwd=package_dir, capture_output=True, text=True)
    if result.returncode != 0:
        detail = (result.stdout + result.stderr).strip()
        raise BuildError(f'writing {SPEC_YML} failed with exit code {result.returncode}'
                         + (f':\n{detail}' if detail else ''))


def expects_tc_output(spec_yml: Path) -> bool:
    """True when the spec's evaluator needs reference output, so generation runs a solution."""
    match = _TC_OUTPUT_PRESENT.search(spec_yml.read_text())
    return match is None or match.group(1) == 'true'


def generate(binary: Path, package_dir: Path, output_dir: Path, solution: Optional[str],
             scorer: Optional[str] = None, communicator: Optional[str] = None,
             extra_args: Sequence[str] = ()) -> None:
    """
    Run the compiled spec from `package_dir` (so program paths resolve as in 1.x),
    writing test cases into `output_dir`. `extra_args` carries multi-file solution flags.
    """
    argv = [str(binary.resolve()), f'--output={output_dir.resolve()}']
    if solution is not None:
        argv.append(f'--solution={solution}')
    if scorer is not None:
        argv.append(f'--scorer={scorer}')
    if communicator is not None:
        argv.append(f'--communicator={communicator}')
    argv.extend(extra_args)
    result = subprocess.run(argv, cwd=package_dir)
    if result.returncode != 0:
        raise BuildError(f'generation failed with exit code {result.returncode}')


def solution_path_for(package_dir: Path, solution: str) -> Path:
    path = Path(solution)
    return path if path.is_absolute() else package_dir / path


def compile_reference(package_dir: Path, build_dir: Path, solution: Path,
                      env: Optional[Mapping[str, str]]) -> Optional[str]:
    """
    Compile a source reference solution (e.g. `solutions/ref/x.cpp`) so `make` works from a
    source package alone (SPEC.md T7.3). Returns a command that runs it from its build
    directory, or None when `solution` is not a source file of a known language.
    """
    languages = available_languages(package_dir, env)
    if not solution.is_file() or language_for(solution, languages) is None:
        return None
    try:
        compiled = compile_program(solution, build_dir / REFERENCE_BUILD_DIR, languages, name=REFERENCE_NAME)
    except CompileError as exc:
        raise BuildError(f'reference solution failed to compile: {exc}')
    run = shlex.join(compiled.argv)
    return shlex.join(['sh', '-c', f'cd {shlex.quote(str(compiled.workdir))} && exec {run}'])


def build_package(
    package_dir: Path,
    build_dir: Optional[Path] = None,
    solution: str = DEFAULT_SOLUTION,
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    env: Optional[Mapping[str, str]] = None,
) -> Path:
    """
    Compile, write `spec.yml`, and generate for the package in `package_dir`.
    Returns the build directory, which holds `spec`, `spec.yml` and `tc/`.

    `solution` is a program command, or a directory for a multi-file solution
    (SPEC.md T6.4). The reference solution is only required when the spec expects
    `.out` files.
    """
    build_dir = build_dir if build_dir is not None else package_dir / DEFAULT_BUILD_DIR
    binary = compile_spec(package_dir, build_dir, env)
    spec_yml = build_dir / SPEC_YML
    emit_spec_yml(binary, package_dir, spec_yml)

    extra_args: list[str] = []
    solution_arg: Optional[str] = anchor_program(package_dir, solution)
    solution_dir = solution_path_for(package_dir, solution)
    compiled_reference = compile_reference(package_dir, build_dir, solution_dir, env)
    if compiled_reference is not None:
        solution_arg = compiled_reference
    if solution_dir.is_dir():
        try:
            extra_args = multifile_args(solution_dir, package_dir, spec_yml,
                                        available_languages(package_dir, env), env)
        except MultifileError as exc:
            raise BuildError(str(exc))
        solution_arg = None
    elif expects_tc_output(spec_yml):
        problem = check_command(solution_arg, 'solution')
        if problem is not None:
            raise BuildError(f'reference solution unavailable: {problem}')

    scorer = resolve_helper(package_dir, build_dir, scorer, 'scorer', env)
    communicator = resolve_helper(package_dir, build_dir, communicator, 'communicator', env)

    generate(binary, package_dir, build_dir / TC_DIR, solution_arg, scorer, communicator, extra_args)
    return build_dir

"""
Automatic compilation of programs (solutions, scorers, communicators, helpers).

A program is either:
  * a single source file, built and run by its language (languages/*.yml),
    picked by file extension; or
  * a directory with a `run` script (required) and a `build` script (optional),
    called as ./build then ./run from inside the directory.

Each program is copied into build_dir/<name>/ before building, so compiler
artifacts never land in the problem package. A failing build raises
CompileError carrying the compiler output.
"""

import os
import shlex
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.languages import Language

BUILD_SCRIPT = 'build'
RUN_SCRIPT = 'run'


class CompileError(Exception):
    """Raised when a program cannot be built or has no runnable entry point; user-facing."""


@dataclass(frozen=True)
class CompiledProgram:
    name: str
    workdir: Path  # run `argv` with this as the current directory
    argv: list[str]


def language_for(path: Path, languages: Mapping[str, Language]) -> Optional[Language]:
    """
    The language claiming `path`'s extension (case-insensitive), or None.
    If several languages claim one extension, the first by slug wins.
    """
    ext = path.suffix.lstrip('.').lower()
    if not ext:
        return None
    for slug in sorted(languages):
        if ext in languages[slug].extensions:
            return languages[slug]
    return None


def compile_program(
    program: Path,
    build_dir: Path,
    languages: Mapping[str, Language],
    name: Optional[str] = None,
) -> CompiledProgram:
    """Build `program` (file or directory) under build_dir/<name>/ and return how to run it."""
    if not program.exists():
        raise CompileError(f"program '{program}' does not exist")
    name = name or program.stem
    if program.is_dir():
        return _compile_dir(program, build_dir / name, name)
    return _compile_file(program, build_dir / name, name, languages)


def _fresh_dir(path: Path) -> Path:
    if path.exists():
        shutil.rmtree(path)
    path.mkdir(parents=True)
    return path


def _compile_file(
    program: Path,
    workdir: Path,
    name: str,
    languages: Mapping[str, Language],
) -> CompiledProgram:
    lang = language_for(program, languages)
    if lang is None:
        ext = program.suffix or '(none)'
        raise CompileError(f"no language for '{program.name}' (extension {ext}); "
                           f"add a languages/<name>.yml that lists this extension")

    workdir = _fresh_dir(workdir)
    shutil.copy2(program, workdir / program.name)
    if lang.build:
        _run_step(lang.build_argv(program.name), workdir, f'build {program.name}')
    return CompiledProgram(name, workdir, lang.run_argv(program.name))


def _compile_dir(program: Path, workdir: Path, name: str) -> CompiledProgram:
    if not (program / RUN_SCRIPT).is_file():
        raise CompileError(f"'{program}' has no '{RUN_SCRIPT}' script")

    workdir = _fresh_dir(workdir)
    shutil.copytree(program, workdir, dirs_exist_ok=True)

    if (workdir / BUILD_SCRIPT).is_file():
        _check_executable(workdir / BUILD_SCRIPT)
        _run_step(['./' + BUILD_SCRIPT], workdir, f'build {name}')
    _check_executable(workdir / RUN_SCRIPT)
    return CompiledProgram(name, workdir, ['./' + RUN_SCRIPT])


def _check_executable(script: Path) -> None:
    if not os.access(script, os.X_OK):
        raise CompileError(f"'{script}' is not executable; try: chmod +x {script}")


def _run_step(argv: list[str], cwd: Path, what: str) -> None:
    try:
        proc = subprocess.run(argv, cwd=cwd, capture_output=True, text=True)
    except OSError as exc:
        raise CompileError(f"{what}: cannot run {argv[0]!r}: {exc}")
    if proc.returncode != 0:
        output = (proc.stdout + proc.stderr).rstrip()
        raise CompileError(f"{what} failed (exit code {proc.returncode})"
                           + (f":\n{output}" if output else ''))


def absolute_command(compiled: CompiledProgram) -> str:
    """Make the compiled command work from any cwd: resolve file arguments inside its build dir."""
    argv = [str((compiled.workdir / arg).resolve()) if (compiled.workdir / arg).exists() else arg
            for arg in compiled.argv]
    return shlex.join(argv)

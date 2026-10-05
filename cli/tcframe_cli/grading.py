"""
Grade many solutions against one built package (T3.2).

Each solution is compiled with compile_program (T2.2) into
build/solutions/<group>-<name>/, then graded by the compiled spec's
`grade --format=json` command (SPEC.md T3.4). The grade runs from that
directory, so a solution command like ./ac-a resolves there. The JSON result
holds the overall verdict, one entry per subtask, and one per test case.
"""

import json
import shlex
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.build import SPEC_BINARY, SPEC_YML, TC_DIR
from tcframe_cli.compiler import CompileError, compile_program
from tcframe_cli.grade import grade_command
from tcframe_cli.languages import Language
from tcframe_cli.multifile import MultifileError, is_multifile, multifile_args
from tcframe_cli.solutions import Solution

SOLUTIONS_BUILD_DIR = 'solutions'
OUTPUT_FORMAT = 'json'


class GradingError(Exception):
    """Raised when the package is not built or grading output cannot be read."""


@dataclass(frozen=True)
class SubtaskResult:
    id: int
    code: str
    points: float


@dataclass(frozen=True)
class CaseOutcome:
    name: str
    code: str
    points: Optional[float]  # None when the test case verdict carries no points


@dataclass(frozen=True)
class Verdict:
    code: str                             # AC, OK, WA, RTE, TLE or ERR
    points: float
    subtasks: tuple[SubtaskResult, ...]
    testcases: tuple[CaseOutcome, ...]


@dataclass(frozen=True)
class SolutionResult:
    solution: Solution
    verdict: Optional[Verdict]  # None when the solution could not be compiled or graded
    error: Optional[str]


def _code_and_points(obj, where: str) -> tuple[str, float]:
    if not isinstance(obj, dict) or not isinstance(obj.get('code'), str):
        raise GradingError(f'{where}: expected an object with a "code"')
    points = obj.get('points')
    if not isinstance(points, (int, float)) or isinstance(points, bool):
        raise GradingError(f'{where}: expected a numeric "points"')
    return obj['code'], float(points)


def parse_json(output: str) -> Verdict:
    """Parse `grade --format=json` stdout into a Verdict."""
    try:
        data = json.loads(output)
    except json.JSONDecodeError as exc:
        raise GradingError(f'grader output is not JSON ({exc}): {output.strip()[:200]!r}')
    if not isinstance(data, dict):
        raise GradingError('grader output is not a JSON object')

    code, points = _code_and_points(data.get('verdict'), 'verdict')

    subtasks = []
    for i, entry in enumerate(data.get('subtasks') or []):
        if not isinstance(entry, dict) or not isinstance(entry.get('id'), int):
            raise GradingError(f'subtasks[{i}]: expected an object with an integer "id"')
        sub_code, sub_points = _code_and_points(entry.get('verdict'), f'subtasks[{i}].verdict')
        subtasks.append(SubtaskResult(entry['id'], sub_code, sub_points))

    testcases = []
    for i, entry in enumerate(data.get('testcases') or []):
        if not isinstance(entry, dict) or not isinstance(entry.get('name'), str):
            raise GradingError(f'testcases[{i}]: expected an object with a "name"')
        tc_code = entry.get('verdict')
        if not isinstance(tc_code, str):
            raise GradingError(f'testcases[{i}]: expected a "verdict" code')
        tc_points = entry.get('points')
        if tc_points is not None and (not isinstance(tc_points, (int, float)) or isinstance(tc_points, bool)):
            raise GradingError(f'testcases[{i}]: "points" must be a number or null')
        testcases.append(CaseOutcome(entry['name'], tc_code,
                                        float(tc_points) if tc_points is not None else None))

    return Verdict(code, points, tuple(subtasks), tuple(testcases))


def _require_built(build_dir: Path) -> tuple[Path, Path]:
    binary = build_dir / SPEC_BINARY
    tc_dir = build_dir / TC_DIR
    if not binary.is_file():
        raise GradingError(f"no compiled spec at '{binary}'; run 'tcframe build' first")
    if not tc_dir.is_dir():
        raise GradingError(f"no test cases at '{tc_dir}'; run 'tcframe build' first")
    return binary.resolve(), tc_dir.resolve()


def _run_grade(argv: list[str], cwd: Path, solution: Solution) -> SolutionResult:
    proc = subprocess.run(argv, cwd=cwd, capture_output=True, text=True)
    if proc.returncode != 0:
        detail = (proc.stdout + proc.stderr).strip()
        return SolutionResult(solution, None,
                              f'grading exited with code {proc.returncode}'
                              + (f':\n{detail}' if detail else ''))
    try:
        return SolutionResult(solution, parse_json(proc.stdout), None)
    except GradingError as exc:
        return SolutionResult(solution, None, str(exc))


def grade_solution(
    solution: Solution,
    build_dir: Path,
    languages: Mapping[str, Language],
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
) -> SolutionResult:
    """
    Compile and grade one solution. `scorer` and `communicator` must be absolute
    paths (or None); the caller resolves them against the package directory.
    `package_dir` locates the manager helper of a multi-file solution (default: the
    parent of build_dir). Failures are returned in SolutionResult.error, not raised.
    """
    if solution.is_dir:
        return _grade_multifile(solution, build_dir, languages, scorer, communicator,
                                package_dir if package_dir is not None else build_dir.parent, env)

    binary, tc_dir = _require_built(build_dir)
    name = f'{solution.group}-{solution.name}'
    try:
        compiled = compile_program(solution.path, build_dir / SOLUTIONS_BUILD_DIR,
                                   languages, name=name)
    except CompileError as exc:
        return SolutionResult(solution, None, str(exc))

    argv = grade_command(binary, tc_dir, shlex.join(compiled.argv),
                         scorer=scorer, communicator=communicator, output_format=OUTPUT_FORMAT)
    return _run_grade(argv, compiled.workdir, solution)


def _grade_multifile(
    solution: Solution,
    build_dir: Path,
    languages: Mapping[str, Language],
    scorer: Optional[str],
    communicator: Optional[str],
    package_dir: Path,
    env: Optional[Mapping[str, str]],
) -> SolutionResult:
    """Grade a directory solution (SPEC.md T6.4): the runner builds it from its key files."""
    binary, tc_dir = _require_built(build_dir)
    spec_yml = build_dir / SPEC_YML
    try:
        if not is_multifile(solution.path, spec_yml, env):
            return SolutionResult(solution, None,
                                  'directory solutions need a functional evaluator (the evaluator takes one solution file)')
        extra = multifile_args(solution.path, package_dir, spec_yml, languages, env)
    except MultifileError as exc:
        return SolutionResult(solution, None, str(exc))

    workdir = build_dir / SOLUTIONS_BUILD_DIR / f'{solution.group}-{solution.name}'
    workdir.mkdir(parents=True, exist_ok=True)
    argv = grade_command(binary, tc_dir, None, scorer=scorer, communicator=communicator,
                         output_format=OUTPUT_FORMAT, extra_args=extra)
    return _run_grade(argv, workdir, solution)


def grade_solutions(
    solutions: list[Solution],
    build_dir: Path,
    languages: Mapping[str, Language],
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    package_dir: Optional[Path] = None,
    env: Optional[Mapping[str, str]] = None,
) -> list[SolutionResult]:
    """Grade every solution in order. Raises GradingError if the package is not built."""
    _require_built(build_dir)
    return [grade_solution(s, build_dir, languages, scorer, communicator, package_dir, env)
            for s in solutions]

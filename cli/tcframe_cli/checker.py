"""
`tcframe test`: grade every solution in `solutions/` and check each against
its directory's expected verdict (RFC "tcframe test").

Match rules:
  * the actual verdict code must be one of the expected verdicts;
  * if the directory has a score (ok-75, tle-75, ...), the actual points must equal it.
A solution that fails to compile or grade is an ERROR, which also fails the run.
"""

from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional

from tcframe_cli.build import DEFAULT_BUILD_DIR, BuildError, resolve_helper
from tcframe_cli.compiler import CompileError, absolute_command, compile_program
from tcframe_cli.grading import SolutionResult, Verdict, grade_solutions
from tcframe_cli.languages import Language, available_languages
from tcframe_cli.make import make_package
from tcframe_cli.solutions import Expected, SolutionsError, parse_solutions

PASS, FAIL, ERROR = 'PASS', 'FAIL', 'ERROR'
_EPSILON = 1e-9


class CheckError(Exception):
    """Raised when the package cannot be built or its reference solution cannot be compiled."""


@dataclass(frozen=True)
class Check:
    result: SolutionResult
    status: str  # PASS, FAIL or ERROR
    detail: str  # one line; for ERROR, the first line of the message

    @property
    def passed(self) -> bool:
        return self.status == PASS


def _points(value: float) -> str:
    return f'{value:g}'


def describe_expected(expected: Expected) -> str:
    text = '/'.join(sorted(expected.verdicts))
    if expected.score is not None:
        text += f' {_points(expected.score)}'
    return text


def check_result(result: SolutionResult) -> Check:
    if result.verdict is None:
        message = (result.error or 'no verdict').splitlines()[0]
        return Check(result, ERROR, message)

    verdict: Verdict = result.verdict
    expected = result.solution.expected
    got = f'{verdict.code} {_points(verdict.points)}'
    verdict_ok = verdict.code in expected.verdicts
    score_ok = expected.score is None or abs(verdict.points - expected.score) < _EPSILON
    if verdict_ok and score_ok:
        return Check(result, PASS, got)
    return Check(result, FAIL, f'got {got}, expected {describe_expected(expected)}')


def run_tests(
    package_dir: Path,
    build_dir: Optional[Path] = None,
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    env: Optional[Mapping[str, str]] = None,
    home: Optional[Path] = None,
) -> list[Check]:
    """
    Build the package using the compiled reference solution, then grade and check
    every solution. Raises CheckError when the build cannot proceed.
    """
    build_dir = build_dir if build_dir is not None else package_dir / DEFAULT_BUILD_DIR
    try:
        solutions = parse_solutions(package_dir)
    except SolutionsError as exc:
        raise CheckError(str(exc))

    languages: dict[str, Language] = available_languages(package_dir, env, home)
    ref = next(s for s in solutions if s.group == 'ref')
    if ref.is_dir:
        ref_command = str(ref.path.resolve())  # multi-file: the runner builds the key files
    else:
        try:
            ref_compiled = compile_program(ref.path, build_dir / 'solutions', languages,
                                           name=f'{ref.group}-{ref.name}')
        except CompileError as exc:
            raise CheckError(f'reference solution: {exc}')
        ref_command = absolute_command(ref_compiled)

    try:
        make_package(package_dir, build_dir, solution=ref_command, env=env)
    except BuildError as exc:
        raise CheckError(str(exc))

    scorer_abs = resolve_helper(package_dir, build_dir, scorer, 'scorer', env)
    communicator_abs = resolve_helper(package_dir, build_dir, communicator, 'communicator', env)
    results = grade_solutions(solutions, build_dir, languages, scorer_abs, communicator_abs,
                              package_dir=package_dir, env=env)
    return [check_result(r) for r in results]


def format_report(checks: list[Check]) -> str:
    """One line per solution, then ERROR details, then a summary line."""
    if not checks:
        return 'no solutions found'
    width = max(len(c.result.solution.label) for c in checks)
    lines = [f'{c.status:<5}  {c.result.solution.label:<{width}}  {c.detail}' for c in checks]

    for c in checks:
        if c.status == ERROR and c.result.error and '\n' in c.result.error:
            lines.append('')
            lines.append(f'{c.result.solution.label}:')
            lines.extend('    ' + line for line in c.result.error.splitlines())

    passed = sum(c.status == PASS for c in checks)
    failed = sum(c.status == FAIL for c in checks)
    errors = sum(c.status == ERROR for c in checks)
    lines.append('')
    lines.append(f'{len(checks)} solutions: {passed} passed, {failed} failed, {errors} errors')
    return '\n'.join(lines)


def exit_code(checks: list[Check]) -> int:
    """0 when every solution matched its expected verdict, else 1."""
    return 0 if all(c.passed for c in checks) else 1

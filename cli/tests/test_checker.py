import shutil
from pathlib import Path

import pytest

from tcframe_cli.checker import (
    ERROR, FAIL, PASS, check_result, describe_expected, exit_code, format_report, run_tests)
from tcframe_cli.cli import main
from tcframe_cli.grading import SolutionResult, Verdict
from tcframe_cli.solutions import AC, OK, RTE, TLE, WA, Expected, Solution

REPO_ROOT = Path(__file__).resolve().parents[2]
GXX = shutil.which('g++')
needs_gxx = pytest.mark.skipif(GXX is None, reason='g++ not available')
ENV = {'TCFRAME_HOME': str(REPO_ROOT)}
SUM = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a + b); }\n'
WRONG = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a - b); }\n'
CRASH = '#include <cstdlib>\nint main(){ std::abort(); }\n'


def solution(group, expected):
    return Solution(group, 'x.cpp', Path('x.cpp'), expected)


def graded(group, expected, code, points=100.0):
    return SolutionResult(solution(group, expected), Verdict(code, points, (), ()), None)


# --- match rules ---------------------------------------------------------------

@pytest.mark.parametrize('expected, code, points, status', [
    (Expected(frozenset({AC}), None), AC, 100, PASS),
    (Expected(frozenset({AC}), None), WA, 0, FAIL),
    (Expected(frozenset({WA, TLE, RTE}), None), TLE, 0, PASS),
    (Expected(frozenset({WA, TLE, RTE}), None), AC, 100, FAIL),
    (Expected(frozenset({OK}), 75), OK, 75, PASS),
    (Expected(frozenset({OK}), 75), OK, 60, FAIL),
    (Expected(frozenset({OK}), None), OK, 33, PASS),
    (Expected(frozenset({TLE}), 75), TLE, 75, PASS),
    (Expected(frozenset({TLE}), 75), TLE, 0, FAIL),
])
def test_match_rules(expected, code, points, status):
    assert check_result(graded('g', expected, code, points)).status == status


def test_fail_detail_names_actual_and_expected():
    check = check_result(graded('wa', Expected(frozenset({WA}), None), AC, 100))
    assert check.detail == 'got AC 100, expected WA'


def test_ok_score_mismatch_detail():
    check = check_result(graded('ok-75', Expected(frozenset({OK}), 75), OK, 60))
    assert check.detail == 'got OK 60, expected OK 75'


def test_error_result_is_error_with_first_line():
    result = SolutionResult(solution('ac', Expected(frozenset({AC}), None)), None,
                            'compile failed:\nline 2\nline 3')
    check = check_result(result)
    assert check.status == ERROR
    assert check.detail == 'compile failed:'


def test_describe_expected_sorts_and_appends_score():
    assert describe_expected(Expected(frozenset({WA, TLE, RTE}), None)) == 'RTE/TLE/WA'
    assert describe_expected(Expected(frozenset({OK}), 75)) == 'OK 75'


def test_exit_code_zero_only_when_all_pass():
    ok = check_result(graded('ac', Expected(frozenset({AC}), None), AC))
    bad = check_result(graded('wa', Expected(frozenset({WA}), None), AC))
    assert exit_code([ok]) == 0
    assert exit_code([ok, bad]) == 1
    assert exit_code([ok, check_result(SolutionResult(bad.result.solution, None, 'x'))]) == 1


def test_report_summary_counts():
    checks = [
        check_result(graded('ac', Expected(frozenset({AC}), None), AC)),
        check_result(graded('wa', Expected(frozenset({WA}), None), AC)),
    ]
    report = format_report(checks)
    assert 'PASS' in report and 'FAIL' in report
    assert report.splitlines()[-1] == '2 solutions: 1 passed, 1 failed, 0 errors'


# --- full pipeline ---------------------------------------------------------------

LANG = """\
name: C++17 (GCC)
family: cpp
extensions: [cc, cpp]
build: {gxx} -std=c++17 -o $BASE_FILENAME $FILENAME
run: ./$BASE_FILENAME
"""


def write(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


@pytest.fixture
def problem(tmp_path):
    pkg = tmp_path / 'aplusb'
    pkg.mkdir()
    write(pkg / 'spec.cpp', (REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    write(pkg / 'languages' / 'cpp17.yml', LANG.format(gxx=GXX or 'g++'))
    write(pkg / 'solutions' / 'ref' / 'ref.cpp', SUM)
    write(pkg / 'solutions' / 'ac' / 'ac.cpp', SUM)
    write(pkg / 'solutions' / 'wa' / 'wa.cpp', WRONG)
    write(pkg / 'solutions' / 'rte' / 'rte.cpp', CRASH)
    return pkg


@needs_gxx
def test_all_matching_solutions_exit_zero(problem):
    checks = run_tests(problem, env=ENV, home=problem / 'nohome')
    by_label = {c.result.solution.label: c for c in checks}

    assert {c.status for c in checks} == {PASS}
    assert by_label['wa/wa.cpp'].detail == 'WA 0'
    assert by_label['rte/rte.cpp'].detail == 'RTE 0'
    assert exit_code(checks) == 0


@needs_gxx
def test_mismatch_fails_the_run(problem):
    write(problem / 'solutions' / 'ac' / 'sneaky.cpp', WRONG)  # claims AC, is WA

    checks = run_tests(problem, env=ENV, home=problem / 'nohome')
    by_label = {c.result.solution.label: c for c in checks}

    assert by_label['ac/sneaky.cpp'].status == FAIL
    assert by_label['ac/sneaky.cpp'].detail == 'got WA 0, expected AC'
    assert exit_code(checks) == 1


@needs_gxx
def test_compile_error_is_error_not_crash(problem):
    write(problem / 'solutions' / 'ac' / 'broken.cpp', 'int main() { return nope; }\n')

    checks = run_tests(problem, env=ENV, home=problem / 'nohome')
    by_label = {c.result.solution.label: c for c in checks}

    assert by_label['ac/broken.cpp'].status == ERROR
    assert by_label['ac/ac.cpp'].status == PASS
    assert exit_code(checks) == 1


@needs_gxx
def test_cli_test_command_exit_code_and_report(problem, monkeypatch, capsys):
    monkeypatch.chdir(problem)
    monkeypatch.setenv('TCFRAME_HOME', str(REPO_ROOT))

    assert main(['test']) == 0
    out = capsys.readouterr().out
    assert 'PASS' in out and 'ref/ref.cpp' in out
    assert out.splitlines()[-1] == '4 solutions: 4 passed, 0 failed, 0 errors'

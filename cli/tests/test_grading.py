import shutil
from pathlib import Path

import pytest

from tcframe_cli.build import build_package
from tcframe_cli.grade import grade_command
from tcframe_cli.grading import (
    GradingError, SubtaskResult, CaseOutcome, grade_solutions, parse_json)
from tcframe_cli.languages import Language
from tcframe_cli.solutions import parse_solutions

REPO_ROOT = Path(__file__).resolve().parents[2]
GXX = shutil.which('g++')
needs_gxx = pytest.mark.skipif(GXX is None, reason='g++ not available')
ENV = {'TCFRAME_HOME': str(REPO_ROOT)}
SUM = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a + b); }\n'
WRONG = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a - b); }\n'
CRASH = '#include <cstdlib>\nint main(){ std::abort(); }\n'


def cpp17():
    return Language('cpp17', 'C++17', 'cpp', ('cc', 'cpp'), build=f'{GXX} -std=c++17 -o $BASE_FILENAME $FILENAME',
                    run='./$BASE_FILENAME', source=Path('cpp17.yml'))


def write(path: Path, text: str):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


@pytest.fixture
def problem(tmp_path):
    pkg = tmp_path / 'aplusb'
    pkg.mkdir()
    (pkg / 'spec.cpp').write_text((REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    write(pkg / 'solutions' / 'ref' / 'ref.cpp', SUM)
    write(pkg / 'solutions' / 'ac' / 'ac.cpp', SUM)
    write(pkg / 'solutions' / 'wa' / 'wa.cpp', WRONG)
    write(pkg / 'solutions' / 'rte' / 'rte.cpp', CRASH)
    write(pkg / 'solutions' / 'ac' / 'broken.cpp', 'int main() { return nope; }\n')
    return pkg


def verdicts_by_label(results):
    return {r.solution.label: r for r in results}


# --- parse_json -------------------------------------------------------------

def test_parse_json_overall_only():
    v = parse_json('{"verdict":{"code":"AC","points":100.00},"subtasks":[],"testcases":[]}')
    assert (v.code, v.points, v.subtasks, v.testcases) == ('AC', 100.0, (), ())


def test_parse_json_subtasks_and_testcases():
    text = ('{"verdict":{"code":"OK","points":40.5},'
            '"subtasks":[{"id":1,"verdict":{"code":"AC","points":25.00}},'
            '{"id":2,"verdict":{"code":"WA","points":0.00}}],'
            '"testcases":[{"name":"a","verdict":"AC","points":null},'
            '{"name":"b","verdict":"WA","points":0.00}]}')
    v = parse_json(text)
    assert v.code == 'OK' and v.points == 40.5
    assert v.subtasks == (SubtaskResult(1, 'AC', 25.0), SubtaskResult(2, 'WA', 0.0))
    assert v.testcases == (CaseOutcome('a', 'AC', None), CaseOutcome('b', 'WA', 0.0))


@pytest.mark.parametrize('text', [
    '',
    'AC 100',
    '[]',
    '{"verdict":{"code":"AC"}}',
    '{"verdict":{"code":"AC","points":1},"subtasks":[{"verdict":{"code":"AC","points":1}}]}',
    '{"verdict":{"code":"AC","points":1},"testcases":[{"name":"x","verdict":"AC","points":"1"}]}',
])
def test_parse_json_rejects_bad_output(text):
    with pytest.raises(GradingError):
        parse_json(text)


def test_grade_command_requests_json_format(tmp_path):
    argv = grade_command(tmp_path / 'spec', tmp_path / 'tc', './x', output_format='json')
    assert '--format=json' in argv
    assert '--format=json' not in grade_command(tmp_path / 'spec', tmp_path / 'tc', './x')


# --- grading a built package ----------------------------------------------------

@needs_gxx
def test_grades_each_solution_through_grade_command(problem):
    solution_sh = problem / 'solution'
    write(solution_sh, '#!/bin/sh\nread a b\necho $((a + b))\n')
    solution_sh.chmod(0o755)
    build_dir = build_package(problem, env=ENV)

    solutions = parse_solutions(problem)
    results = verdicts_by_label(grade_solutions(solutions, build_dir, {'cpp17': cpp17()}))

    assert results['ref/ref.cpp'].verdict.code == 'AC'
    assert results['ac/ac.cpp'].verdict.code == 'AC'
    assert results['ac/ac.cpp'].verdict.points == 100
    assert results['wa/wa.cpp'].verdict.code == 'WA'
    assert results['rte/rte.cpp'].verdict.code == 'RTE'
    assert results['wa/wa.cpp'].error is None


@needs_gxx
def test_compile_error_is_reported_per_solution(problem):
    write(problem / 'solution', '#!/bin/sh\nread a b\necho $((a + b))\n')
    (problem / 'solution').chmod(0o755)
    build_dir = build_package(problem, env=ENV)

    results = verdicts_by_label(grade_solutions(parse_solutions(problem), build_dir, {'cpp17': cpp17()}))

    broken = results['ac/broken.cpp']
    assert broken.verdict is None
    assert 'nope' in broken.error
    assert results['ac/ac.cpp'].verdict.code == 'AC'  # other solutions still graded


@needs_gxx
def test_directory_solution_needs_functional_evaluator(problem):
    (problem / 'solutions' / 'ref' / 'ref.cpp').unlink()
    (problem / 'solutions' / 'ref' / 'fushar').mkdir()
    write(problem / 'solutions' / 'ref' / 'fushar' / 'a.cpp', SUM)
    write(problem / 'solution', '#!/bin/sh\nread a b\necho $((a + b))\n')
    (problem / 'solution').chmod(0o755)
    build_dir = build_package(problem, env=ENV)

    result = grade_solutions(parse_solutions(problem)[:1], build_dir, {'cpp17': cpp17()},
                             package_dir=problem, env=ENV)[0]
    assert result.verdict is None
    assert 'functional evaluator' in result.error


def test_unbuilt_package_is_error(problem, tmp_path):
    with pytest.raises(GradingError, match="run 'tcframe build' first"):
        grade_solutions([], tmp_path / 'build', {})

import subprocess
from pathlib import Path

import pytest

from tcframe_cli.compiler import compile_program
from tcframe_cli.paths import find_resource, REGISTRY
from tcframe_cli.simple_yaml import parse

REGISTRY_DIR = Path(__file__).resolve().parents[2] / 'registry'


@pytest.fixture
def build(tmp_path):
    return tmp_path / 'build'


def run_program(name, build, stdin, args=()):
    """Compile a registry directory program with T2.2 and run it with args and stdin."""
    compiled = compile_program(REGISTRY_DIR / name, build, {})
    proc = subprocess.run(compiled.argv + list(args), cwd=compiled.workdir,
                          input=stdin, capture_output=True, text=True)
    assert proc.returncode == 0, proc.stderr
    return proc


def aggregate(agg, build, points, verdicts, extra=()):
    stdin = f'{len(verdicts)}\n' + ''.join(v + '\n' for v in verdicts)
    return run_program(f'aggregators/{agg}', build, stdin,
                       [str(points), *extra]).stdout.strip()


# --- layout and config -------------------------------------------------------

def test_registry_layout_present():
    for rel in ['aggregators/min/run', 'aggregators/sum/run', 'aggregators/threshold/run',
                'helpers/scorer/compare/run', 'helpers/scorer/defaults.yml',
                'evaluators/batch/evaluator.yml', 'evaluators/interactive/evaluator.yml']:
        assert (REGISTRY_DIR / rel).is_file(), rel


def test_registry_resolves_through_paths(tmp_path):
    found = find_resource(REGISTRY, 'helpers/scorer/compare/run', home=tmp_path,
                          env={'TCFRAME_HOME': str(REGISTRY_DIR.parent)})
    assert found == REGISTRY_DIR / 'helpers' / 'scorer' / 'compare' / 'run'


def test_scorer_defaults_name_compare():
    assert parse((REGISTRY_DIR / 'helpers/scorer/defaults.yml').read_text()) == {'slug': 'compare'}


def test_batch_evaluator_config():
    data = parse((REGISTRY_DIR / 'evaluators/batch/evaluator.yml').read_text())
    assert data == {
        'custom_solution_keys': 'false',
        'tc_output': 'optional',
        'helpers': [{'slug': 'scorer', 'type': 'program', 'optional': 'true'}],
    }


def test_interactive_evaluator_config():
    data = parse((REGISTRY_DIR / 'evaluators/interactive/evaluator.yml').read_text())
    assert data['tc_output'] == 'not_required'
    assert data['helpers'] == [{'slug': 'communicator', 'type': 'program', 'optional': 'false'}]


# --- aggregators -------------------------------------------------------------

@pytest.mark.parametrize('verdicts, expected', [
    (['AC', 'AC'], 'AC'),
    (['AC', 'OK 40'], 'OK 40'),
    (['OK 30', 'OK 60'], 'OK 30'),
    (['OK 20', 'WA'], 'WA'),
    (['AC', 'TLE'], 'TLE'),
    (['OK 10', 'RTE', 'WA'], 'RTE'),
    ([], 'AC'),
])
def test_min_aggregator(build, verdicts, expected):
    assert aggregate('min', build, 100, verdicts) == expected


@pytest.mark.parametrize('verdicts, expected', [
    (['AC', 'AC', 'AC', 'AC'], 'AC'),
    (['AC', 'AC', 'OK 10', 'OK 20'], 'OK 80'),
    (['AC', 'AC', 'OK 10', 'WA'], 'WA 60'),
    (['AC', 'TLE', 'AC', 'AC'], 'TLE 75'),
    ([], 'AC'),
])
def test_sum_aggregator(build, verdicts, expected):
    assert aggregate('sum', build, 100, verdicts) == expected


@pytest.mark.parametrize('verdicts, expected', [
    (['AC', 'OK 18'], 'AC'),
    (['AC', 'OK 19'], 'WA'),
    (['OK 5', 'RTE'], 'RTE'),
    (['OK 19', 'TLE'], 'TLE'),
    (['OK'], 'WA'),
    ([], 'AC'),
])
def test_threshold_aggregator(build, verdicts, expected):
    assert aggregate('threshold', build, 25, verdicts, extra=['18']) == expected


# --- default scorer ----------------------------------------------------------

def score(build, expected_text, actual_text):
    compiled = compile_program(REGISTRY_DIR / 'helpers/scorer/compare', build, {})
    (compiled.workdir / 'exp.out').write_text(expected_text)
    (compiled.workdir / 'act.out').write_text(actual_text)
    return subprocess.run(['./run', 'in.txt', 'exp.out', 'act.out'], cwd=compiled.workdir,
                          capture_output=True, text=True)


def test_compare_scorer_accepts_identical(build):
    proc = score(build, '1 2\n', '1 2\n')
    assert proc.stdout == 'AC\n'
    assert proc.stderr == ''


def test_compare_scorer_reports_diff_on_wrong_answer(build):
    proc = score(build, '1 2\n', '1 3\n')
    assert proc.stdout == 'WA\n'
    assert 'Diff:' in proc.stderr
    assert '(expected) [line 01]' in proc.stderr
    assert '(received) [line 01]' in proc.stderr


def score_with(build, options, expected_text, actual_text):
    compiled = compile_program(REGISTRY_DIR / 'helpers/scorer/compare', build, {})
    (compiled.workdir / 'exp.out').write_text(expected_text)
    (compiled.workdir / 'act.out').write_text(actual_text)
    return subprocess.run(['./run', *options, 'in.txt', 'exp.out', 'act.out'], cwd=compiled.workdir,
                          capture_output=True, text=True)


def test_compare_scorer_ignore_whitespace(build):
    assert score_with(build, ['ignore_whitespace'], '1 2\n', '1   2\n').stdout == 'AC\n'
    assert score_with(build, [], '1 2\n', '1   2\n').stdout == 'WA\n'


def test_compare_scorer_absolute_tolerance(build):
    assert score_with(build, ['float_absolute_tolerance', '1e-9'], '0.5\n', '0.5000000001\n').stdout == 'AC\n'
    proc = score_with(build, ['float_absolute_tolerance', '1e-9'], '0.5\n', '0.6\n')
    assert proc.stdout == 'WA\n'
    assert "Token 1: expected '0.5', received '0.6'" in proc.stderr


def test_compare_scorer_relative_tolerance_scales(build):
    # Absolute difference 1, relative difference 1e-9.
    assert score_with(build, ['float_relative_tolerance', '1e-8'],
                      '1000000000\n', '1000000001\n').stdout == 'AC\n'
    assert score_with(build, ['float_absolute_tolerance', '1e-8'],
                      '1000000000\n', '1000000001\n').stdout == 'WA\n'


def test_compare_scorer_both_tolerances_accept_either(build):
    opts = ['float_absolute_tolerance', '1e-6', 'float_relative_tolerance', '1e-12']
    assert score_with(build, opts, '0\n', '1e-7\n').stdout == 'AC\n'
    assert score_with(build, opts, '1000000000\n', '1000000001\n').stdout == 'WA\n'


def test_compare_scorer_float_mode_ignores_layout(build):
    opts = ['float_absolute_tolerance', '1e-9']
    assert score_with(build, opts, '1 2\n3\n', '1\t2 3').stdout == 'AC\n'


def test_compare_scorer_float_mode_token_count(build):
    proc = score_with(build, ['float_absolute_tolerance', '1e-9'], '1 2\n', '1\n')
    assert proc.stdout == 'WA\n'
    assert 'Token count: expected 2, received 1' in proc.stderr


def test_compare_scorer_float_mode_non_numbers_exact(build):
    proc = score_with(build, ['float_absolute_tolerance', '1'], 'yes\n', 'no\n')
    assert proc.stdout == 'WA\n'
    assert "Token 1: expected 'yes', received 'no'" in proc.stderr


def test_compare_scorer_rejects_unknown_option(build):
    proc = score_with(build, ['bogus'], '1\n', '1\n')
    assert proc.returncode == 2
    assert 'unknown option: bogus' in proc.stderr
